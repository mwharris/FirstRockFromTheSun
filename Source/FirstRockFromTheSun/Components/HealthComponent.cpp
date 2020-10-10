#include "HealthComponent.h"
#include "Engine/World.h"
#include "FirstRockFromTheSun/GameModes/BP_GameMode.h"
#include "Kismet/GameplayStatics.h"

UHealthComponent::UHealthComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}

/*
// TODO: Is this needed?
void UHealthComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}
*/

void UHealthComponent::BeginPlay()
{
	Super::BeginPlay();

	CurrentHealth = MaxHealth;
	// Get a reference to our custom game mode
	GameModeRef = Cast<ABP_GameMode>(UGameplayStatics::GetGameMode(GetWorld()));
	// Bind the OnTakeDamage event to our TakeDamage() function
	GetOwner()->OnTakeAnyDamage.AddDynamic(this, &UHealthComponent::TakeDamage);
}

void UHealthComponent::TakeDamage(AActor* DamagedActor, float Damage, const UDamageType* DamageType, AController* InstigatedBy, AActor* DamageCauser) 
{
	UE_LOG(LogTemp, Warning, TEXT("Taking Damage!"));
	
	CurrentHealth -= Damage;
	
	if (CurrentHealth <= 0) 
	{
		// Die
	}
	else
	{
		// Keep on truckin'
	}
}