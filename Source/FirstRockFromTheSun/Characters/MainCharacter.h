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
	USceneComponent* RotatePoint;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
	USceneComponent* FlareRaycastPoint;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
	UHealthComponent* HealthComponent;
	UPROPERTY(EditDefaultsOnly, Category = "Damage")
	TSubclassOf<UDamageType> DamageType;
	UPROPERTY(EditAnywhere, Category = "Damage")
	float FlareDamage = 1.f;
	UPROPERTY(EditAnywhere, Category = "Movement")
	float RotationRate = 0.75f;
	UPROPERTY(VisibleAnywhere, Category = "Movement")
	float RotationAmount = 0.f;
	UPROPERTY(EditAnywhere, Category="Effects")
	USoundBase* BurningSound;
	UPROPERTY(EditAnywhere, Category="Effects")
	USoundBase* JumpSound;

	ABP_GameMode* GameModeRef;
	UCharacterMovementComponent* MovementComponent;
	UCapsuleComponent* CapsuleComponent;
	UAudioComponent* BurningSoundComponent;
	bool IsAlive = true;
	bool GameOver = false;

	void Move(float AxisValue);
	void CustomJump();
	void WallJump();
	void ToggleMissionList();
	void RestartLevel();

};