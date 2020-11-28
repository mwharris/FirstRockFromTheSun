#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "MissionPoint.generated.h"

class AMainCharacter;
class UBoxComponent;
class UTextRenderComponent;
class ABP_GameMode;

UCLASS()
class FIRSTROCKFROMTHESUN_API AMissionPoint : public AActor
{
	GENERATED_BODY()
	
public:	
	AMissionPoint();
	
	bool GetMissionComplete() const;
	bool IsFinalMission() const;

	UFUNCTION(BlueprintCallable, Category="Mission")
	FString GetMissionListText() const;
	UFUNCTION(BlueprintCallable, Category="Mission")
	int32 GetMissionNumber() const;

protected:
	virtual void BeginPlay() override;

private:
	UPROPERTY(EditAnywhere, Category="Components")
	UBoxComponent* BoxCollider = nullptr;
	UPROPERTY(EditAnywhere, Category="Components")
	UTextRenderComponent* TitleTextRender;
	UPROPERTY(EditAnywhere, Category="Components")
	UTextRenderComponent* SuccessTextRender;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Properties", meta=(AllowPrivateAccess = "true"))
	int32 MissionNumber = 0;
	UPROPERTY(EditAnywhere, Category="Properties")
	bool FinalMission = false;
	UPROPERTY(VisibleAnywhere, Category="Properties")
	bool MissionComplete = false;
	
	UPROPERTY(EditAnywhere, Category="Text")
	FString Objective = "Do something";
	UPROPERTY(EditAnywhere, Category="Text")
	FString ItemName = "tacos";
	UPROPERTY(EditAnywhere, Category="Text")
	FString CharacterName = "Malibel";

	UPROPERTY(EditAnywhere, Category="Sounds")
	USoundBase* PickupSound;
	
	ABP_GameMode* GameModeRef;
	AMainCharacter* Player;
	FString From = TEXT(" from ");
	FString House = TEXT(" house.");
	FString Done = TEXT(" (DONE)");

	UFUNCTION()
	void OnBeginOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, 
		int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

};
