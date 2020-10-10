#include "BP_GameMode.h"
#include "Kismet/GameplayStatics.h"
#include "TimerManager.h"
#include "FirstRockFromTheSun/Characters/MainCharacter.h"

ABP_GameMode::ABP_GameMode() 
{
    PrimaryActorTick.bCanEverTick = true;
}

void ABP_GameMode::BeginPlay()
{
	Super::BeginPlay();
    // Get a reference to the player
    Player = Cast<AMainCharacter>(UGameplayStatics::GetPlayerPawn(GetWorld(), 0));
    // Start the initial solar flare timer
    GetWorldTimerManager().SetTimer(FlareTimerHandle, this, &ABP_GameMode::StartSolarFlare, SolarFlareDowntime);
}

void ABP_GameMode::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
    // If our solar flare is active, tell the player to raycast
    if (SolarFlareActive) 
    {
        Player->DoSolarFlareRaycast(DeltaTime);
    }
}

// Take us out of "Solar Flare" state and into "Downtime" state
void ABP_GameMode::StartDowntime() 
{
    UE_LOG(LogTemp, Warning, TEXT("Downtime!"));
    SolarFlareActive = false;
    GetWorldTimerManager().SetTimer(FlareTimerHandle, this, &ABP_GameMode::StartSolarFlare, SolarFlareDowntime);
}

// Put us into "Solar Flare" state
void ABP_GameMode::StartSolarFlare() 
{
    UE_LOG(LogTemp, Warning, TEXT("SOLAR FLARE!"));
    SolarFlareActive = true;
    GetWorldTimerManager().SetTimer(FlareTimerHandle, this, &ABP_GameMode::StartDowntime, SolarFlareDuration);
}

// Helper function accessible in blueprints
bool ABP_GameMode::IsSolarFlareActive() const
{
    return SolarFlareActive;
}