#include "BP_GameMode.h"
#include "Components/AudioComponent.h"
#include "FirstRockFromTheSun/Actors/MissionPoint.h"
#include "FirstRockFromTheSun/Characters/MainCharacter.h"
#include "Kismet/GameplayStatics.h"
#include "TimerManager.h"

ABP_GameMode::ABP_GameMode() 
{
    PrimaryActorTick.bCanEverTick = true;
}

void ABP_GameMode::BeginPlay()
{
	Super::BeginPlay();
    // Get a reference to the player
    Player = Cast<AMainCharacter>(UGameplayStatics::GetPlayerPawn(GetWorld(), 0));
    // Get references to all Mission Points in the level
    LoadMissionPoints();
}

void ABP_GameMode::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
    // Get the name of the current level so we don't run logic on the Main Menu
    FString CurrLevelName = UGameplayStatics::GetCurrentLevelName(GetWorld());
    // Don't bother checking this stuff until we're playing the game
    if (GameStarted) 
    {
        CheckWinCondition();
        // If our solar flare is active, tell the player to raycast
        if (SolarFlareActive) 
        {
            Player->DoSolarFlareRaycast(DeltaTime);
        }
    }
    // Start our timers only when we start the play level
    else if (CurrLevelName == TEXT("GameMap"))
    {
        GameStarted = true;
        StartDowntime();
    }
}

// Get a list of spawned MissionPoints and convert to AMissionPoints
void ABP_GameMode::LoadMissionPoints() 
{
	TArray<AActor*> MPActors;
    UGameplayStatics::GetAllActorsOfClass(GetWorld(), AMissionPoint::StaticClass(), MPActors);
    if (MPActors.Num() > 0) 
    {
        for (AActor* MPActor : MPActors) 
        {
            MissionPoints.Add(Cast<AMissionPoint>(MPActor));
        }
    }
}

void ABP_GameMode::CheckWinCondition() 
{
    if (MissionPoints.Num() == 0) { return; }
    if (AllMissionsComplete()) 
    {
        HandleGameOver(false);
    }
}

// Helper function to determine if all of our missions are complete
bool ABP_GameMode::AllMissionsComplete() const
{
    for (AMissionPoint* MissionPoint : MissionPoints) 
    {
        if (!MissionPoint->GetMissionComplete()) 
        {
            return false;
        }
    }
    return true;
}

// Solar Flare -> Downtime
void ABP_GameMode::StartDowntime() 
{
    CountdownActive = false;
    SolarFlareActive = false;
    if (FlareSoundComponent) {
        FlareSoundComponent->Stop();
    }
    GetWorldTimerManager().SetTimer(FlareTimerHandle, this, &ABP_GameMode::StartCountdown, SolarFlareDowntime);
}

// Downtime -> Countdown
void ABP_GameMode::StartCountdown() 
{
    CountdownActive = true;
    SolarFlareActive = false;
    AlarmSoundComponent = UGameplayStatics::SpawnSoundAtLocation(GetWorld(), AlarmSound, Player->GetActorLocation(), FRotator::ZeroRotator);
    GetWorldTimerManager().SetTimer(FlareTimerHandle, this, &ABP_GameMode::StartSolarFlare, SolarFlareCountdown);
}

// Countdown -> Solar Flare
void ABP_GameMode::StartSolarFlare() 
{
    CountdownActive = false;
    SolarFlareActive = true;
    if (AlarmSoundComponent) 
    {
        AlarmSoundComponent->Stop();
    }
    FlareSoundComponent = UGameplayStatics::SpawnSoundAtLocation(GetWorld(), FlareSound, Player->GetActorLocation(), FRotator::ZeroRotator);
    GetWorldTimerManager().SetTimer(FlareTimerHandle, this, &ABP_GameMode::StartDowntime, SolarFlareDuration);
}

// Handle move to Win or Loss state
void ABP_GameMode::HandleGameOver(bool PlayerDied) 
{
    // Move to downtime but clear our timer
    if (GetWorld() && FlareTimerHandle.IsValid())
    {
        GetWorldTimerManager().ClearTimer(FlareTimerHandle);
    }
    SolarFlareActive = false;
    CountdownActive = false;
    // Tell the player to handle player-specific operations
    Player->HandleGameOver(PlayerDied);
    // Mark the game as being over
    GameOver = true;
    // Set the player won flag accordingly
    if (PlayerDied) 
    {
        PlayerWon = false;
    }
    else 
    {
        PlayerWon = true;
    }
}

// The Final Mission should only complete when all other missions are completed
bool ABP_GameMode::ShouldFinalMissionComplete() const
{
    bool AllOtherComplete = false;
    for (AMissionPoint* MissionPoint : MissionPoints) 
    {
        if (!MissionPoint->IsFinalMission() && !MissionPoint->GetMissionComplete()) 
        {
            return false;
        }
    }
    return true;
}

void ABP_GameMode::ToggleMissionList() 
{
    MissionListOpen = !MissionListOpen;
}

// GETTER FUNCTIONS
bool ABP_GameMode::IsSolarFlareActive() const { return SolarFlareActive; }
bool ABP_GameMode::IsCountdownActive()  const { return CountdownActive; }
bool ABP_GameMode::IsGameOver()         const { return GameOver; }
bool ABP_GameMode::DidPlayerWin()       const { return PlayerWon; }
bool ABP_GameMode::IsMissionListOpen()  const { return MissionListOpen; }