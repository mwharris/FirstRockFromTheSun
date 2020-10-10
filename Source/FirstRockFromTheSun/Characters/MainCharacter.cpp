#include "MainCharacter.h"
#include "Components/InputComponent.h"
#include "DrawDebugHelpers.h"

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
}

void AMainCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	PlayerInputComponent->BindAxis(TEXT("Horizontal"), this, &AMainCharacter::Move);
}

void AMainCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AMainCharacter::Move(float AxisValue) 
{
	AddMovementInput(GetActorForwardVector() * AxisValue);
}

// Perform the Solar Flare raycast towards our player and deal damage if necessary
void AMainCharacter::DoSolarFlareRaycast() 
{
	// Perform the raycast
	FHitResult Hit;
	bool bSuccess = GetWorld()->LineTraceSingleByChannel(
		Hit, 
		FlareRaycastPoint->GetComponentLocation(), 
		TempMesh->GetComponentLocation(), 
		ECC_Visibility
	);
	// Check if the sun's rays hit the player
	if (bSuccess && Hit.Actor == this)
	{
		// TODO: Damage the player
	}
	DrawDebugLine(GetWorld(), FlareRaycastPoint->GetComponentLocation(), TempMesh->GetComponentLocation(), FColor::Yellow);
}