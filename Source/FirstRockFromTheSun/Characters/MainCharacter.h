#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "MainCharacter.generated.h"

class UHealthComponent;
class ABP_GameMode;

UCLASS()
class FIRSTROCKFROMTHESUN_API AMainCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	AMainCharacter();
	virtual void Tick(float DeltaTime) override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	void DoSolarFlareRaycast(float DeltaTime);
	void HandleGameOver(bool PlayerDied);
	bool GetIsAlive() const;

protected:
	virtual void BeginPlay() override;

private:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
	UStaticMeshComponent* TempMesh;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
	USceneComponent* FlareRaycastPoint;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
	UHealthComponent* HealthComponent;
	UPROPERTY(EditDefaultsOnly, Category = "Damage")
	TSubclassOf<UDamageType> DamageType;
	UPROPERTY(EditAnywhere, Category = "Damage")
	float FlareDamage = 1.f;

	ABP_GameMode* GameModeRef;
	UCharacterMovementComponent* MovementComponent;
	UCapsuleComponent* CapsuleComponent;
	bool IsAlive = true;

	void Move(float AxisValue);
	void CustomJump();
	void WallJump();
	void ToggleMissionList();

};