// Fill out your copyright notice in the Description page of Project Settings.


#include "KilledAllGameMode.h"
#include "GameFramework/Controller.h"
#include "EngineUtils.h"
#include "SimpleShooter/ShooterAIController.h"

void AKilledAllGameMode::PawnKilled(APawn* PawnKilled)
{
    Super::PawnKilled(PawnKilled);

    APlayerController* PlayerController = Cast<APlayerController>(PawnKilled->GetController());
    if (PlayerController != nullptr)
    {
        EndGame(false);
    }

    for (AShooterAIController* AIController : TActorRange<AShooterAIController>(GetWorld()))
    {
        if (!AIController->IsDead())
        {
            // didn't win
            return;
        }
    }

    // all ai dead, player wins
    EndGame(true);
}

void AKilledAllGameMode::EndGame(bool bPlayerWon)
{
    for (AController* Controller : TActorRange<AController>(GetWorld()))
    {
        if (Controller->IsPlayerController())
        {
            // if controller is player controller
            if (bPlayerWon)
            {
                // if player won then call GameHasEnded(nullptr, true) on the player controller
                Controller->GameHasEnded(Controller->GetPawn(), true);
            }
            else
            {
                // player lost
                Controller->GameHasEnded(Controller->GetPawn(), false);
            }
        }
        else
        {
            // if controller is ai controller
            if (bPlayerWon)
            {
                // if player won then call GameHasEnded(nullptr, false) on the ai controller (because if player won, then ai lost)
                Controller->GameHasEnded(Controller->GetPawn(), false);
            }
            else 
            {
                // player lost, ai won
                Controller->GameHasEnded(Controller->GetPawn(), true);
            }
        }
    }   
}
