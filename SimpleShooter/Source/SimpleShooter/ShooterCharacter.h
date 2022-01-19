// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "ShooterCharacter.generated.h"

class AGun;

UCLASS()
class SIMPLESHOOTER_API AShooterCharacter : public ACharacter
{
	GENERATED_BODY()

protected:
	virtual void BeginPlay() override;

public:	
	AShooterCharacter();
	virtual void Tick(float DeltaTime) override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const &DamageEvent, class AController *EventInstigator, AActor *DamageCauser) override;
	void Shoot();

	UFUNCTION(BlueprintPure)
	bool IsDead() const;

	UFUNCTION(BlueprintPure)
	float GetHealthPercent() const;

	void AddAmmoForGunIndex(int Amount, int GunIndex);
	int GetActiveGunIndex();
	
private:
	void MoveForward(float AxisValue);
	// void LookUp(float AxisValue);
	void MoveRight(float AxisValue);
	// void LookRight(float AxisValue);
	void LookUpRate(float AxisValue);
	void LookRightRate(float AxisValue);
	void ScrollWeapon(float AxisValue);
	void DisableGun(AGun* Gun, bool ToHide);

	UPROPERTY(EditAnywhere)
	float RotationRate = 10.0f;
	
	UPROPERTY(EditDefaultsOnly)
	float MaxHealth = 100.0f;

	UPROPERTY(VisibleAnywhere)
	float Health;

	UPROPERTY(EditDefaultsOnly)
	TArray<TSubclassOf<AGun>> GunClassArray;

	UPROPERTY()
	TArray<AGun*> GunArray;

	UPROPERTY(EditAnywhere)
	int ActiveGunIndex = 0;
};
