// Fill out your copyright notice in the Description page of Project Settings.


#include "AmmoPickup.h"
#include "Components/SphereComponent.h"
#include "Components/StaticMeshComponent.h"
#include "SimpleShooter/ShooterCharacter.h"
#include "Kismet/GameplayStatics.h"

AAmmoPickup::AAmmoPickup()
{
	PrimaryActorTick.bCanEverTick = true;

	Root = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	SetRootComponent(Root);

	SphereComponent = CreateDefaultSubobject<USphereComponent>(TEXT("SphereComponent"));
	SphereComponent->SetupAttachment(Root);
	
	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	Mesh->SetupAttachment(SphereComponent);
}

void AAmmoPickup::BeginPlay()
{
	Super::BeginPlay();
	
	Player = Cast<AShooterCharacter>(UGameplayStatics::GetPlayerPawn(GetWorld(), 0));
}

void AAmmoPickup::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (IsOverlappingActor(Player))
	{
		PickUp();
		GetWorldTimerManager().SetTimer(ReenablePickupTimer, this, &AAmmoPickup::ReenablePickup, ReenablePickupDelay);
	}
}

void AAmmoPickup::PickUp()
{
	// Give player ammo, disable collision, hide mesh
	Player->AddAmmoForGunIndex(AmmoSupply, AmmoForGunAtIndex);
	SetActorEnableCollision(false);
	SetActorHiddenInGame(true);	
}

void AAmmoPickup::ReenablePickup()
{
	SetActorEnableCollision(true);
	SetActorHiddenInGame(false);
}
