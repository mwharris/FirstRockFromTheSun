#include "MainCharacter.h"
#include "Components/AudioComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/InputComponent.h"
#include "DrawDebugHelpers.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "FirstRockFromTheSun/GameModes/BP_GameMode.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"

AMainCharacter::AMainCharacter()
{
	PrimaryActorTick.bCanEverTick = true;

	RotatePoint = CreateDefaultSubobject<USceneComponent>(TEXT("Mesh Rotate Point"));
	RotatePoint->SetupAttachment(RootComponent);

	TempMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("TempMesh"));
	TempMesh->SetupAttachment(RotatePoint);

	FlareRaycastPoint = CreateDefaultSubobject<USceneComponent>(TEXT("Flare Raycast Point"));
	FlareRaycastPoint->SetupAttachment(RootComponent);

	AudioComponent = CreateDefaultSubobject<UAudioComponent>(TEXT("Burn Audio"));
	AudioComponent->SetupAttachment(RootComponent);
}

void AMainCharacter::BeginPlay()
{
	Super::BeginPlay();
	GameModeRef = Cast<ABP_GameMode>(UGameplayStatics::GetGameMode(GetWorld()));
	MovementComponent = GetCharacterMovement();
	CapsuleComponent = GetCapsuleComponent();
}

void AMainCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	PlayerInputComponent->BindAxis(TEXT("Horizontal"), this, &AMainCharacter::Move);
	PlayerInputComponent->BindAction(TEXT("Jump"), IE_Pressed, this, &AMainCharacter::CustomJump);
	PlayerInputComponent->BindAction(TEXT("OpenMissionList"), IE_Pressed, this, &AMainCharacter::ToggleMissionList);
	PlayerInputComponent->BindAction(TEXT("RestartLevel"), IE_Pressed, this, &AMainCharacter::RestartLevel);
}

void AMainCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	// Determine velocity and speed ignoring Z
	FVector Velocity = GetVelocity();
	FVector FlatVelocity = FVector(Velocity.X, Velocity.Y, 0.f);
	float CurrentSpeed = FlatVelocity.Size();
	// Only rotate if we're moving
	if (CurrentSpeed != 0.f) 
	{
		// Calculate the smoothed rotation amount
		float SpeedOverPiRate = CurrentSpeed / (PI * RotationRate);
		RotationAmount = RotationAmount + (DeltaTime * SpeedOverPiRate);
		// Create a rotator we can use to rotate our mesh
		FRotator RotFromX = UKismetMathLibrary::MakeRotFromX(FlatVelocity);
		FVector RightVec = UKismetMathLibrary::GetRightVector(RotFromX);
		FRotator WorldRot = UKismetMathLibrary::RotatorFromAxisAndAngle(RightVec, RotationAmount);
		FString Str = WorldRot.Vector().ToString();
		// Rotate the RotatePoint (midpoint)
		RotatePoint->SetWorldRotation(WorldRot);
	}
}

void AMainCharacter::Move(float AxisValue) 
{
	if (!IsAlive) { return; }
	AddMovementInput(GetActorForwardVector() * AxisValue);
}

void AMainCharacter::ToggleMissionList() 
{
	if (!IsAlive) { return; }
	NotifyHUDMissionList();
}

void AMainCharacter::CustomJump() 
{
	if (!IsAlive) { return; }
	if (MovementComponent != nullptr && !MovementComponent->IsFalling()) 
	{
		Jump();
	}
	else 
	{
		WallJump();
	}
	UGameplayStatics::PlaySoundAtLocation(GetWorld(), JumpSound, GetActorLocation());
}

// Check if we collided with a wall and can wall jump
void AMainCharacter::WallJump() 
{
	FHitResult Hit;
	// Calculate a length for a wall jump raycast
	FVector Forward = GetActorForwardVector();
	// Make sure it's slightly bigger than our radius
	FVector Length = Forward * (CapsuleComponent->GetScaledCapsuleRadius() + 10.f);
	// Raycast from (Location - Length) to (Location + Length)
	FVector Start = RotatePoint->GetComponentLocation() - Length;
	FVector End = RotatePoint->GetComponentLocation() + Length;
	// Make sure we don't hit ourselves
	FCollisionQueryParams CollisionParams;
	CollisionParams.AddIgnoredActor(this);
	// Perform the raycast and check the results
	bool bSuccess = GetWorld()->LineTraceSingleByChannel(Hit, Start, End, ECC_GameTraceChannel1, CollisionParams);
	if (bSuccess) 
	{
		// Subtract player location from impact point in order to determine what side we hit the wall
		float LaunchVelocityX = FMath::Sign(RotatePoint->GetComponentLocation().X - Hit.ImpactPoint.X) * 800.f;
		float LaunchVelocityZ = 1400;
		// Launch our character in the appropriate direction
		LaunchCharacter(FVector(LaunchVelocityX, 0.f, LaunchVelocityZ), true, true);
	}
}

// Perform the Solar Flare raycast towards our player and deal damage if necessary
void AMainCharacter::DoSolarFlareRaycast(float DeltaTime) 
{
	// Perform the raycast to the center of the player
	FHitResult Hit;
	FVector Start = FlareRaycastPoint->GetComponentLocation();
	FVector End = RotatePoint->GetComponentLocation();
	// Check if the sun's rays hit the player
	bool bSuccess = GetWorld()->LineTraceSingleByChannel(Hit, Start, End, ECC_Visibility);
	if (bSuccess && Hit.GetActor() && Hit.GetActor() == this)
	{
		// Apply damage and play a sound to indicate we're being hurt
		UGameplayStatics::ApplyDamage(Hit.GetActor(), FlareDamage * DeltaTime, GetInstigatorController(), this, DamageType);
		if (!AudioComponent->IsPlaying()) {
			AudioComponent->Play();
		}
	}
	else if (AudioComponent->IsPlaying()) {
		AudioComponent->Stop();
	}
}

// Called by GameMode to make sure we're not still playing audio
void AMainCharacter::StopSolarFlareAudio() const
{
	AudioComponent->Stop();
}

void AMainCharacter::RestartLevel() 
{
	if (!GameOver) { return; }
	UGameplayStatics::OpenLevel(GetWorld(), "GameMap", true);
}

void AMainCharacter::HandleGameOver(bool PlayerDied) 
{
	IsAlive = !PlayerDied;
	GameOver = true;
	NotifyHUDGameOver();
}

bool AMainCharacter::GetIsAlive() const
{
	return IsAlive;
}