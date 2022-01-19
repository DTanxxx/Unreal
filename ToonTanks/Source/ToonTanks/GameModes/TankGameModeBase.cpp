// Fill out your copyright notice in the Description page of Project Settings.


#include "TankGameModeBase.h"
#include "ToonTanks/Pawns/PawnTank.h"
#include "ToonTanks/Pawns/PawnTurret.h"
#include "ToonTanks/PlayerControllers/PlayerControllerBase.h"
#include "Kismet/GameplayStatics.h"

void ATankGameModeBase::BeginPlay()
{
    Super::BeginPlay();

    // Call HandleGameStart() to initialise the start countdown, turret activation, pawn check etc.
    HandleGameStart();
}

void ATankGameModeBase::HandleGameStart()
{
    // Get references 
    TargetTurrets = GetTargetTurretCount();
    PlayerTank = Cast<APawnTank>(UGameplayStatics::GetPlayerPawn(this, 0));
    PlayerControllerRef = Cast<APlayerControllerBase>(UGameplayStatics::GetPlayerController(this, 0));

    // Initialise the start countdown, turret activation, pawn check etc.

    // Call Blueprint version GameStart() -> this is only responsible for displaying the widget, whereas the widget itself
    // is responsible for the countdown timing functionality; therefore GameStart() function does not wait for the countdown 
    // to finish and instead executes following lines of code immediately 
    GameStart();

    if (PlayerControllerRef)
    {
        // Disable player input when countdown starts
        PlayerControllerRef->SetPlayerEnabledState(false);

        // Enable player input after a duration of StartDelay time (ie when countdown ends)
        FTimerHandle PlayerEnableHandle;
        FTimerDelegate PlayerEnableDelegate = FTimerDelegate::CreateUObject(
            PlayerControllerRef,
            &APlayerControllerBase::SetPlayerEnabledState,
            true
        );

        GetWorld()->GetTimerManager().SetTimer(
            PlayerEnableHandle, 
            PlayerEnableDelegate, 
            StartDelay, 
            false
        );

        // Show score widget after StartDelay time
        FTimerHandle ShowScoreHandle;
        GetWorld()->GetTimerManager().SetTimer(
            ShowScoreHandle,
            this,
            &ATankGameModeBase::ShowScore,
            StartDelay,
            false
        );
    }
}

void ATankGameModeBase::HandleGameOver(bool PlayerWon)
{
    // See if the player has destroyed all the turrets, show win result
    // else if turret destroyed player, show lose result
    
    // Call Blueprint version GameOver(bool)
    GameOver(PlayerWon);
}

void ATankGameModeBase::ActorDied(AActor* DeadActor)
{
    // Check what type of Actor died; if Turret, tally; if Player, go to lose condition
    if (DeadActor == PlayerTank)
    {
        PlayerTank->HandleDestruction();
        HandleGameOver(false);

        // Disable player input when player is dead
        if (PlayerControllerRef)
        {
            PlayerControllerRef->SetPlayerEnabledState(false);
        }
    }
    else if (APawnTurret* DestroyedTurret = Cast<APawnTurret>(DeadActor))
    {
        Score += DestroyedTurret->GetScoreReward();
        UpdateScore(Score);
        DestroyedTurret->HandleDestruction();

        if (--TargetTurrets == 0)
        {
            HandleGameOver(true);
        }
    }
}

int32 ATankGameModeBase::GetTargetTurretCount()
{
    TArray<AActor*> TurretActors;
    UGameplayStatics::GetAllActorsOfClass(GetWorld(), APawnTurret::StaticClass(), TurretActors);
    return TurretActors.Num();
}

