#include "MissionPoint.h"
#include "Components/BoxComponent.h"
#include "FirstRockFromTheSun/Characters/MainCharacter.h"
#include "Kismet/GameplayStatics.h"

AMissionPoint::AMissionPoint()
{
	PrimaryActorTick.bCanEverTick = false;

	BoxCollider = CreateDefaultSubobject<UBoxComponent>(TEXT("Box Collider"));
	SetRootComponent(BoxCollider);
}

void AMissionPoint::BeginPlay()
{
	Super::BeginPlay();
    // Get a reference to our player
	Player = Cast<AMainCharacter>(UGameplayStatics::GetPlayerPawn(GetWorld(), 0));
	// Hook into the component overlap event
	BoxCollider->OnComponentBeginOverlap.AddDynamic(this, &AMissionPoint::OnBeginOverlap);
}

void AMissionPoint::OnBeginOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, 
		int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult) 
{
	// Ignore the event if we're already completed this mission
	if (!MissionComplete) 
	{
		// If it was the player that overlapped, mark the mission as complete
		if (OtherActor && Player && OtherActor == Player) 
		{
			MissionComplete = true;
		}
	}
}

int32 AMissionPoint::GetMissionNumber() const
{
	return MissionNumber;
}

bool AMissionPoint::GetMissionComplete() const
{
	return MissionComplete;
}