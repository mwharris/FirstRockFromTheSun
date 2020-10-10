#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "BP_GameMode.generated.h"

class AMainCharacter;

UCLASS()
class FIRSTROCKFROMTHESUN_API ABP_GameMode : public AGameModeBase
{
	GENERATED_BODY()

public:
	virtual void Tick(float DeltaTime) override;

	UFUNCTION(BlueprintCallable, Category="Solar Flare")
	bool IsSolarFlareActive() const;
	
protected:
	virtual void BeginPlay() override;

private:
	UPROPERTY(EditAnywhere)
	float SolarFlareDowntime = 10.f;

	UPROPERTY(EditAnywhere)
	float SolarFlareDuration = 10.f;
	
	AMainCharacter* Player;
	FTimerHandle FlareTimerHandle;
	bool SolarFlareActive = false;

	void StartDowntime();
	void StartSolarFlare();
	void EndSolarFlare();

};
