#include "MainCharacter.h"
#include "Components/CapsuleComponent.h"
#include "Components/InputComponent.h"
#include "DrawDebugHelpers.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "FirstRockFromTheSun/GameModes/BP_GameMode.h"
#include "Kismet/GameplayStatics.h"

AMainCharacter::AMainCharacter()
{
	PrimaryActorTick.bCanEverTick = true;

	TempMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("TempMesh"));
	TempMesh->SetupAttachment(RootComponent);

	FlareRaycastPoint = CreateDefaultSubobject<USceneComponent>(TEXT("Flare Raycast Point"));
	FlareRaycastPoint->SetupAttachment(RootComponent);
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
}

void AMainCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AMainCharacter::Move(float AxisValue) 
{
	AddMovementInput(GetActorForwardVector() * AxisValue);
}

void AMainCharacter::ToggleMissionList() 
{
	GameModeRef->ToggleMissionList();
}

void AMainCharacter::CustomJump() 
{
	if (!MovementComponent->IsFalling()) 
	{
		Jump();
	}
	else 
	{
		WallJump();
	}
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
	FVector Start = TempMesh->GetComponentLocation() - Length;
	FVector End = TempMesh->GetComponentLocation() + Length;
	// Make sure we don't hit ourselves
	FCollisionQueryParams CollisionParams;
	CollisionParams.AddIgnoredActor(this);
	// Perform the raycast and check the results
	bool bSuccess = GetWorld()->LineTraceSingleByChannel(Hit, Start, End, ECC_GameTraceChannel1, CollisionParams);
	if (bSuccess) 
	{
		// Subtract player location from impact point in order to determine what side we hit the wall
		float LaunchVelocityX = FMath::Sign(TempMesh->GetComponentLocation().X - Hit.ImpactPoint.X) * 800.f;
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
	FVector MeshLoc = TempMesh->GetComponentLocation();
	FVector Start = FlareRaycastPoint->GetComponentLocation();
	FVector End = FVector(MeshLoc.X, MeshLoc.Y, MeshLoc.Z + CapsuleComponent->GetScaledCapsuleRadius());
	// Check if the sun's rays hit the player
	bool bSuccess = GetWorld()->LineTraceSingleByChannel(Hit, Start, End, ECC_Visibility);
	if (bSuccess && Hit.GetActor())
	{
		if (Hit.GetActor() == this) 
		{
			UGameplayStatics::ApplyDamage(Hit.GetActor(), FlareDamage * DeltaTime, GetInstigatorController(), this, DamageType);
		}
	}
}

void AMainCharacter::HandleGameOver(bool PlayerDied) 
{
	IsAlive = !PlayerDied;
}

bool AMainCharacter::GetIsAlive() const
{
	return IsAlive;
}