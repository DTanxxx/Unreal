// Fill out your copyright notice in the Description page of Project Settings.


#include "ShooterCharacter.h"
#include "SimpleShooter/Gun.h"
#include "Components/CapsuleComponent.h"
#include "SimpleShooterGameModeBase.h"

AShooterCharacter::AShooterCharacter()
{
	PrimaryActorTick.bCanEverTick = true;
}

void AShooterCharacter::BeginPlay()
{
	Super::BeginPlay();

	Health = MaxHealth;
	
	for (int i = 0; i < GunClassArray.Num(); ++i)
	{
		AGun* Gun = GetWorld()->SpawnActor<AGun>(GunClassArray[i]);
		GetMesh()->HideBoneByName(FName(TEXT("weapon_r")), EPhysBodyOp::PBO_None);
		Gun->AttachToComponent(GetMesh(), FAttachmentTransformRules::KeepRelativeTransform, FName(TEXT("WeaponSocket")));
		Gun->SetOwner(this);
		GunArray.Add(Gun);
		if (i != ActiveGunIndex)
		{
			DisableGun(Gun, true);
		}
	}
}

void AShooterCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void AShooterCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAxis(FName(TEXT("MoveForward")), this, &AShooterCharacter::MoveForward);
	PlayerInputComponent->BindAxis(FName(TEXT("LookUp")), this, &APawn::AddControllerPitchInput);
	PlayerInputComponent->BindAxis(FName(TEXT("MoveRight")), this, &AShooterCharacter::MoveRight);
	PlayerInputComponent->BindAxis(FName(TEXT("LookRight")), this, &APawn::AddControllerYawInput);
	PlayerInputComponent->BindAxis(FName(TEXT("LookUpRate")), this, &AShooterCharacter::LookUpRate);
	PlayerInputComponent->BindAxis(FName(TEXT("LookRightRate")), this, &AShooterCharacter::LookRightRate);
	PlayerInputComponent->BindAxis(FName(TEXT("ScrollWeapon")), this, &AShooterCharacter::ScrollWeapon);
	PlayerInputComponent->BindAction(FName(TEXT("Jump")), EInputEvent::IE_Pressed, this, &ACharacter::Jump);
	PlayerInputComponent->BindAction(FName(TEXT("Shoot")), EInputEvent::IE_Pressed, this, &AShooterCharacter::Shoot);
}

void AShooterCharacter::MoveForward(float AxisValue)
{
	AddMovementInput(GetActorForwardVector() * AxisValue);
}

// void AShooterCharacter::LookUp(float AxisValue)
// {
// 	AddControllerPitchInput(AxisValue);
// }

void AShooterCharacter::MoveRight(float AxisValue)
{
	AddMovementInput(GetActorRightVector() * AxisValue);
}

// void AShooterCharacter::LookRight(float AxisValue)
// {
// 	AddControllerYawInput(AxisValue);
// }

void AShooterCharacter::LookUpRate(float AxisValue) 
{
	AddControllerPitchInput(AxisValue * RotationRate * GetWorld()->DeltaTimeSeconds);
}

void AShooterCharacter::LookRightRate(float AxisValue) 
{
	AddControllerYawInput(AxisValue * RotationRate * GetWorld()->DeltaTimeSeconds);
}

void AShooterCharacter::ScrollWeapon(float AxisValue)
{
	DisableGun(GunArray[ActiveGunIndex], true);
	// Get an Index from 0 to GunArray length - 1
	ActiveGunIndex = int(ActiveGunIndex + AxisValue);
	if (ActiveGunIndex >= GunArray.Num())
	{
		ActiveGunIndex = GunArray.Num() - 1;
	}
	else if (ActiveGunIndex < 0)
	{
		ActiveGunIndex = 0;
	}
	DisableGun(GunArray[ActiveGunIndex], false);
}

void AShooterCharacter::DisableGun(AGun* Gun, bool ToHide)
{
	Gun->SetActorHiddenInGame(ToHide);
	Gun->SetActorEnableCollision(!ToHide);
	Gun->SetActorTickEnabled(!ToHide);
}

void AShooterCharacter::Shoot()
{
	GunArray[ActiveGunIndex]->PullTrigger();
}

bool AShooterCharacter::IsDead() const
{
	return Health <= 0.0f;
}

float AShooterCharacter::GetHealthPercent() const
{
	return Health / MaxHealth;
}

float AShooterCharacter::TakeDamage(float DamageAmount, struct FDamageEvent const &DamageEvent, class AController *EventInstigator, AActor *DamageCauser) 
{
	float DamageToApply = Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);
	DamageToApply = FMath::Min(Health, DamageToApply);
	Health -= DamageToApply;
	UE_LOG(LogTemp, Warning, TEXT("%f"), Health);

	if (IsDead())
	{
		// Call GameMode method
		ASimpleShooterGameModeBase* GameMode = GetWorld()->GetAuthGameMode<ASimpleShooterGameModeBase>();
		if (GameMode != nullptr)
		{
			GameMode->PawnKilled(this);
		}

		DetachFromControllerPendingDestroy();
		GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	}

	return DamageToApply;
}

void AShooterCharacter::AddAmmoForGunIndex(int Amount, int GunIndex)
{
	if (GunIndex >= GunArray.Num() || GunIndex < 0)
	{
		UE_LOG(LogTemp, Warning, TEXT("%i gun index does not exist in GunArray!"), GunIndex);
		return;
	}
	
	GunArray[GunIndex]->AddAmmo(Amount);
}

int AShooterCharacter::GetActiveGunIndex()
{
	return ActiveGunIndex;
}
