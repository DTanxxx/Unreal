// Fill out your copyright notice in the Description page of Project Settings.


#include "Gun.h"
#include "Components/SkeletalMeshComponent.h"
#include "Kismet/GameplayStatics.h"
#include "DrawDebugHelpers.h"

AGun::AGun()
{
	PrimaryActorTick.bCanEverTick = true;

	Root = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	SetRootComponent(Root);
	
	Mesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Mesh"));
	Mesh->SetupAttachment(Root);
}

void AGun::BeginPlay()
{
	Super::BeginPlay();
	
}

void AGun::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AGun::PullTrigger()
{
	if (Ammo == 0)
	{
		// Play empty sound effect
		UGameplayStatics::SpawnSoundAttached(EmptyMagSound, Mesh, FName(TEXT("MagSocket")));
		return;
	}

	// Spawn particle and sound effects
	UGameplayStatics::SpawnEmitterAttached(MuzzleFlash, Mesh, FName(TEXT("MuzzleFlashSocket")));
	UGameplayStatics::SpawnSoundAttached(MuzzleSound, Mesh, FName(TEXT("MuzzleFlashSocket")));

	// Subtract ammo
	--Ammo;
	
	FHitResult Hit;
	FVector ShotDirection;
	bool bSuccess = GunTrace(Hit, ShotDirection);
	
	if (bSuccess)
	{
		UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), ImpactEffect, Hit.Location, ShotDirection.Rotation());
		UGameplayStatics::PlaySoundAtLocation(GetWorld(), ImpactSound, Hit.Location);

		AActor* HitActor = Hit.GetActor();
		if (HitActor != nullptr)
		{
			FPointDamageEvent DamageEvent(Damage, Hit, ShotDirection, nullptr);
			AController* OwnerController = GetOwnerController();
			HitActor->TakeDamage(Damage, DamageEvent, OwnerController, this);
		}
		// DrawDebugPoint(GetWorld(), Hit.Location, 20.0f, FColor::Red, true);		
	}
	// DrawDebugCamera(GetWorld(), Location, Rotation, 90.0f, 2.0f, FColor::White, true);
}

bool AGun::GunTrace(FHitResult& Hit, FVector& ShotDirection)
{
	AController* OwnerController = GetOwnerController();
	if (OwnerController == nullptr) { return false; }

	FVector Location;
	FRotator Rotation;
	OwnerController->GetPlayerViewPoint(Location, Rotation);

	ShotDirection = -Rotation.Vector();

	FVector End = Location + Rotation.Vector() * MaxRange;

	FCollisionQueryParams Params;
	Params.AddIgnoredActor(this);
	Params.AddIgnoredActor(GetOwner());

	return GetWorld()->LineTraceSingleByChannel(Hit, Location, End, ECollisionChannel::ECC_GameTraceChannel1, Params);
}

AController* AGun::GetOwnerController() const
{
	APawn* OwnerPawn = Cast<APawn>(GetOwner());  // we called Gun->SetOwner(this) in ShooterCharacter.cpp in BeginPlay()
	if (OwnerPawn == nullptr) { return nullptr; }
	return OwnerPawn->GetController();
}

void AGun::AddAmmo(int Amount)
{
	Ammo += Amount;
}
