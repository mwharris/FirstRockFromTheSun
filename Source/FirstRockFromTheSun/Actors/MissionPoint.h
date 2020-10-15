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
	UPROPERTY(EditAnywhere)
	UBoxComponent* BoxCollider = nullptr;
	UPROPERTY(EditAnywhere)
	UTextRenderComponent* TitleTextRender;
	UPROPERTY(EditAnywhere)
	UTextRenderComponent* SuccessTextRender;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta=(AllowPrivateAccess = "true"))
	int32 MissionNumber = 0;
	UPROPERTY(EditAnywhere)
	bool FinalMission = false;
	UPROPERTY(EditAnywhere)
	FString Objective = "Do something";
	UPROPERTY(EditAnywhere)
	FString ItemName = "tacos";
	UPROPERTY(EditAnywhere)
	FString CharacterName = "Malibel";
	
	UPROPERTY(VisibleAnywhere)
	bool MissionComplete = false;
	
	ABP_GameMode* GameModeRef;
	AMainCharacter* Player;
	FString From = TEXT(" from ");
	FString House = TEXT(" house.");
	FString Done = TEXT(" (DONE)");

	UFUNCTION()
	void OnBeginOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, 
		int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

};
