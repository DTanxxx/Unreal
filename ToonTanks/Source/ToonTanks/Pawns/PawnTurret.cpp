// Fill out your copyright notice in the Description page of Project Settings.


#include "PawnTurret.h"
#include "Kismet/GameplayStatics.h"
#include "PawnTank.h"

APawnTurret::APawnTurret()
{

}

void APawnTurret::BeginPlay()
{
	Super::BeginPlay();
	
    GetWorld()->GetTimerManager().SetTimer(
        FireRateTimerHandle, 
        this, 
        &APawnTurret::CheckFireCondition, 
        FireRate, 
        true
    );

    PlayerPawn = Cast<APawnTank>(UGameplayStatics::GetPlayerPawn(this, 0));
}

void APawnTurret::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

    if (!PlayerPawn || ReturnDistanceToPlayer() > FireRange)
    {
        return;
    }

    RotateTurret(PlayerPawn->GetActorLocation());
}

void APawnTurret::CheckFireCondition()
{
    // Check if player is dead or null
    if (!PlayerPawn || !PlayerPawn->GetIsPlayerAlive()) 
    {
        return;
    }

    // Check if player is in range
    if (ReturnDistanceToPlayer() <= FireRange)
    {
        // Fire
        Fire();
    }
}

float APawnTurret::ReturnDistanceToPlayer()
{
    if (!PlayerPawn)
    {
        return 0.0f;
    }

    return FVector::Dist(GetActorLocation(), PlayerPawn->GetActorLocation());
}

void APawnTurret::HandleDestruction()
{
    // Call base pawn class HandleDestruction to play effects
    Super::HandleDestruction();

    Destroy();
}

int32 APawnTurret::GetScoreReward()
{
    return ScoreReward;
}

