#include "SolarFlare.h"
#include "Components/DirectionalLightComponent.h"
#include "Engine/World.h"
#include "FirstRockFromTheSun/GameModes/BP_GameMode.h"
#include "Kismet/GameplayStatics.h"

ASolarFlare::ASolarFlare()
{
	PrimaryActorTick.bCanEverTick = true;

	Location = CreateDefaultSubobject<USceneComponent>(TEXT("Scene Component"));
	SetRootComponent(Location);

	LightSource = CreateDefaultSubobject<UDirectionalLightComponent>(TEXT("Light Source"));
	LightSource->SetupAttachment(RootComponent);
}

void ASolarFlare::BeginPlay()
{
	Super::BeginPlay();
	GameModeRef = Cast<ABP_GameMode>(UGameplayStatics::GetGameMode(GetWorld()));
}

void ASolarFlare::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}