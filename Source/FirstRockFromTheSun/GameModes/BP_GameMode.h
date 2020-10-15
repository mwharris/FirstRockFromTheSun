#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "BP_GameMode.generated.h"

class AMainCharacter;
class UAudioComponent;
class AMissionPoint;

UCLASS()
class FIRSTROCKFROMTHESUN_API ABP_GameMode : public AGameModeBase
{
	GENERATED_BODY()

public:
	ABP_GameMode();
	virtual void Tick(float DeltaTime) override;
	void HandleGameOver(bool PlayerDied);
	void ToggleMissionList();
	bool ShouldFinalMissionComplete() const;

	UFUNCTION(BlueprintCallable, Category="Solar Flare")
	bool IsSolarFlareActive() const;
	UFUNCTION(BlueprintCallable, Category="Solar Flare")
	bool IsCountdownActive() const;
	UFUNCTION(BlueprintCallable, Category="Game State")
	bool IsGameOver() const;
	UFUNCTION(BlueprintCallable, Category="Game State")
	bool DidPlayerWin() const;
	UFUNCTION(BlueprintCallable, Category="Game State")
	bool IsMissionListOpen() const;
	
protected:
	virtual void BeginPlay() override;

private:
	UPROPERTY(EditDefaultsOnly)
	float SolarFlareDowntime = 10.f;
	UPROPERTY(EditDefaultsOnly)
	float SolarFlareCountdown = 5.f;
	UPROPERTY(EditDefaultsOnly)
	float SolarFlareDuration = 10.f;
	UPROPERTY(EditAnywhere, Category="Effects")
	USoundBase* AlarmSound;
	
	// References and variable cache
	AMainCharacter* Player;
	TArray<AMissionPoint*> MissionPoints;
	FTimerHandle FlareTimerHandle;
	UAudioComponent* AlarmSoundComponent;
	bool GameStarted = false;

	// Variables for various game states
	bool CountdownActive = false;
	bool SolarFlareActive = false;
	bool GameOver = false;
	bool PlayerWon = false;
	bool MissionListOpen = false;

	void LoadMissionPoints();
	void CheckWinCondition();
	bool AllMissionsComplete() const;
	void StartDowntime();
	void StartCountdown();
	void StartSolarFlare();
	void EndSolarFlare();

};
