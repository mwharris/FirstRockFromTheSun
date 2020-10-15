#include "KillBox.h"
#include "Components/BoxComponent.h"
#include "FirstRockFromTheSun/Characters/MainCharacter.h"
#include "FirstRockFromTheSun/Components/HealthComponent.h"
#include "Kismet/GameplayStatics.h"

AKillBox::AKillBox()
{
	PrimaryActorTick.bCanEverTick = false;
	BoxCollider = CreateDefaultSubobject<UBoxComponent>(TEXT("Box Collider"));
	SetRootComponent(BoxCollider);
}

void AKillBox::BeginPlay()
{
	Super::BeginPlay();
	Player = Cast<AMainCharacter>(UGameplayStatics::GetPlayerPawn(GetWorld(), 0));
	BoxCollider->OnComponentBeginOverlap.AddDynamic(this, &AKillBox::OnBeginOverlap);
}

void AKillBox::OnBeginOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, 
		int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult) 
{
	if (OtherActor == Player) 
	{
		UHealthComponent* HealthComponent = Player->FindComponentByClass<UHealthComponent>();
		UGameplayStatics::ApplyDamage(Player, HealthComponent->GetCurrentHealth(), GetInstigatorController(), this, DamageType);
	}
}