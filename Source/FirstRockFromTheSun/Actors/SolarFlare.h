#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "SolarFlare.generated.h"

class ABP_GameMode;
class UDirectionalLightComponent;

UCLASS()
class FIRSTROCKFROMTHESUN_API ASolarFlare : public AActor
{
	GENERATED_BODY()

public:
	ASolarFlare();
	virtual void Tick(float DeltaTime) override;

protected:
	virtual void BeginPlay() override;

private:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta=(AllowPrivateAccess = "true"))
	USceneComponent* Location;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta=(AllowPrivateAccess = "true"))
	UDirectionalLightComponent* LightSource;

	ABP_GameMode* GameModeRef;

};
