// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "AmmoPickup.generated.h"

class USphereComponent;
class AShooterCharacter;

UCLASS()
class SIMPLESHOOTER_API AAmmoPickup : public AActor
{
	GENERATED_BODY()
	
public:	
	AAmmoPickup();
	virtual void Tick(float DeltaTime) override;

protected:
	virtual void BeginPlay() override;

private:
	UPROPERTY(VisibleAnywhere)
	USceneComponent* Root;

	UPROPERTY(VisibleAnywhere)
	USphereComponent* SphereComponent;

	UPROPERTY(VisibleAnywhere)
	UStaticMeshComponent* Mesh;

	UPROPERTY(EditAnywhere)
	int AmmoSupply = 5;

	UPROPERTY(EditAnywhere)
	int AmmoForGunAtIndex = 0;

	UPROPERTY(EditAnywhere)
	float ReenablePickupDelay = 2.5f; 

	AShooterCharacter* Player;
	FTimerHandle ReenablePickupTimer;

	void PickUp();
	void ReenablePickup();

};
