#include "HealthComponent.h"
#include "Engine/World.h"
#include "FirstRockFromTheSun/GameModes/BP_GameMode.h"
#include "Kismet/GameplayStatics.h"

UHealthComponent::UHealthComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}

void UHealthComponent::BeginPlay()
{
	Super::BeginPlay();
	// Initialize our starting HP
	CurrentHealth = MaxHealth;
	// Get a reference to our custom game mode
	GameModeRef = Cast<ABP_GameMode>(UGameplayStatics::GetGameMode(GetWorld()));
	// Bind the OnTakeDamage event to our TakeDamage() function
	GetOwner()->OnTakeAnyDamage.AddDynamic(this, &UHealthComponent::TakeDamage);
}

void UHealthComponent::TakeDamage(AActor* DamagedActor, float Damage, const UDamageType* DamageType, AController* InstigatedBy, AActor* DamageCauser) 
{
	CurrentHealth -= Damage;
	if (CurrentHealth <= 0) 
	{
		GameModeRef->HandleGameOver(true);
	}
}