#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "MissionPoint.generated.h"

class UBoxComponent;
class AMainCharacter;

UCLASS()
class FIRSTROCKFROMTHESUN_API AMissionPoint : public AActor
{
	GENERATED_BODY()
	
public:	
	AMissionPoint();

	int32 GetMissionNumber() const;
	bool GetMissionComplete() const;

protected:
	virtual void BeginPlay() override;

private:
	UPROPERTY(EditAnywhere)
	UBoxComponent* BoxCollider = nullptr;
	UPROPERTY(EditAnywhere)
	int32 MissionNumber = 0;
	UPROPERTY(VisibleAnywhere)
	bool MissionComplete = false;
	
	AMainCharacter* Player;

	UFUNCTION()
	void OnBeginOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, 
		int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

};
