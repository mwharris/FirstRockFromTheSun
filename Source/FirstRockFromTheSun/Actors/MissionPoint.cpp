#include "MissionPoint.h"
#include "Components/BoxComponent.h"
#include "Components/TextRenderComponent.h"
#include "FirstRockFromTheSun/Characters/MainCharacter.h"
#include "Kismet/GameplayStatics.h"

AMissionPoint::AMissionPoint()
{
	PrimaryActorTick.bCanEverTick = false;

	BoxCollider = CreateDefaultSubobject<UBoxComponent>(TEXT("Box Collider"));
	SetRootComponent(BoxCollider);

	TitleTextRender = CreateDefaultSubobject<UTextRenderComponent>(TEXT("Title Text Render"));
	TitleTextRender->SetupAttachment(RootComponent);

	SuccessTextRender = CreateDefaultSubobject<UTextRenderComponent>(TEXT("Success Text Render"));
	SuccessTextRender->SetVisibility(false);
	SuccessTextRender->SetupAttachment(RootComponent);
}

void AMissionPoint::BeginPlay()
{
	Super::BeginPlay();
    // Get a reference to our player
	Player = Cast<AMainCharacter>(UGameplayStatics::GetPlayerPawn(GetWorld(), 0));
	// Hook into the component overlap event
	BoxCollider->OnComponentBeginOverlap.AddDynamic(this, &AMissionPoint::OnBeginOverlap);
	// Set our text render values
	if (FinalMission) 
	{
		TitleTextRender->SetText(FText::FromString(TEXT("Your House")));
		SuccessTextRender->SetText(FText::FromString(TEXT("tacos get!")));
	}
	else
	{
		TitleTextRender->SetText(FText::FromString(CharacterName + TEXT("'s House")));
		SuccessTextRender->SetText(FText::FromString(ItemName + TEXT(" get!")));
	}
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
			SuccessTextRender->SetVisibility(true);
		}
	}
}

FString AMissionPoint::GetMissionListText() const
{
	FString ReturnString;
	if (FinalMission)
	{
		ReturnString = "Go home and make tacos!";
	} 
	else 
	{
		ReturnString = Dash + Objective + TEXT(" ") + ItemName + From + CharacterName;
	}
	if (MissionComplete)
	{
		ReturnString += Done;
	}
	return ReturnString;
}

int32 AMissionPoint::GetMissionNumber() const
{
	return MissionNumber;
}

bool AMissionPoint::GetMissionComplete() const
{
	return MissionComplete;
}