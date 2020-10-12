#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "MissionPoint.generated.h"

class AMainCharacter;
class UBoxComponent;
class UTextRenderComponent;

UCLASS()
class FIRSTROCKFROMTHESUN_API AMissionPoint : public AActor
{
	GENERATED_BODY()
	
public:	
	AMissionPoint();

	int32 GetMissionNumber() const;
	bool GetMissionComplete() const;

	UFUNCTION(BlueprintCallable, Category="Mission")
	FString GetMissionListText() const;

protected:
	virtual void BeginPlay() override;

private:
	UPROPERTY(EditAnywhere)
	UBoxComponent* BoxCollider = nullptr;
	UPROPERTY(EditAnywhere)
	UTextRenderComponent* TextRenderComp;
	UPROPERTY(EditAnywhere)
	int32 MissionNumber = 0;
	UPROPERTY(EditAnywhere)
	FString Objective = "Do something";
	UPROPERTY(EditAnywhere)
	FString CharacterName = "Malibel";
	
	UPROPERTY(VisibleAnywhere)
	bool MissionComplete = false;
	
	AMainCharacter* Player;

	UFUNCTION()
	void OnBeginOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, 
		int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

};
