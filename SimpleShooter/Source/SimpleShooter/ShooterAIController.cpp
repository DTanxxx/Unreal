// Fill out your copyright notice in the Description page of Project Settings.


#include "ShooterAIController.h"
#include "Kismet/GameplayStatics.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "SimpleShooter/ShooterCharacter.h"

void AShooterAIController::BeginPlay()
{
    Super::BeginPlay();

    if (AIBehavior != nullptr)
    {
        RunBehaviorTree(AIBehavior);
        GetBlackboardComponent()->SetValueAsVector(FName(TEXT("StartLocation")), GetPawn()->GetActorLocation());
    }
}

void AShooterAIController::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);
}

bool AShooterAIController::IsDead() const
{
    AShooterCharacter* ControlledAICharacter = Cast<AShooterCharacter>(GetPawn());
    if (ControlledAICharacter != nullptr)
    {
        return ControlledAICharacter->IsDead();        
    }

    // since we are detaching the ai controller from the character when it dies, we may have nullptr for GetPawn()
    // in that case, we return true because the ai character was already dead
    return true;
}

