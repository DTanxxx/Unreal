// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "PawnBase.generated.h"

class UCapsuleComponent;
class AProjectileBase;
class UHealthComponent;

UCLASS()
class TOONTANKS_API APawnBase : public APawn
{
	GENERATED_BODY()

private:
	// COMPONENTS
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
	UCapsuleComponent* CapsuleComp;  // a root component (for the hierarchy)
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
	UStaticMeshComponent* BaseMesh;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
	UStaticMeshComponent* TurretMesh;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
	USceneComponent* ProjectileSpawnPoint;  // used to represent a spawning location in the world
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
	UHealthComponent* HealthComponent;
	// VARIABLES
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Projectile Type", meta = (AllowPrivateAccess = "true"))
	TSubclassOf<AProjectileBase> ProjectileClass;
	UPROPERTY(EditAnywhere, Category = "Effects")
	UParticleSystem* DeathParticle;
	UPROPERTY(EditAnywhere, Category = "Effects")
	USoundBase* DeathSound;
	UPROPERTY(EditAnywhere, Category = "Effects")
	TSubclassOf<UMatineeCameraShake> DeathShake;

public:
	// Sets default values for this pawn's properties
	APawnBase();

	virtual void HandleDestruction();

	// Below public variables are for demo purposes, showing how different access modifiers work
	// UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "DEMO")
	// int VisibleAnywhere;
	// UPROPERTY(VisibleDefaultsOnly, Category = "DEMO")
	// int VisibleDefaultsOnly;
	// UPROPERTY(VisibleInstanceOnly, Category = "DEMO")
	// int VisibleInstanceOnly;
	// UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "DEMO")
	// int EditAnywhere;
	// UPROPERTY(EditDefaultsOnly, Category = "DEMO")
	// int EditDefaultsOnly;
	// UPROPERTY(EditInstanceOnly, Category = "DEMO")
	// int EditInstanceOnly;

protected:
	void RotateTurret(FVector LookAtTarget);
	void Fire();
};
