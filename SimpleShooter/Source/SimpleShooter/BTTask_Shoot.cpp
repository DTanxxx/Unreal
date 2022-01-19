// Fill out your copyright notice in the Description page of Project Settings.


#include "BTTask_Shoot.h"
#include "SimpleShooter/ShooterCharacter.h"
#include "AIController.h"

UBTTask_Shoot::UBTTask_Shoot()
{
    NodeName = TEXT("Shoot");
}

EBTNodeResult::Type UBTTask_Shoot::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
    Super::ExecuteTask(OwnerComp, NodeMemory);

    if (OwnerComp.GetAIOwner() == nullptr)
    {
        return EBTNodeResult::Failed;
    }

    AShooterCharacter* AICharacter = Cast<AShooterCharacter>(OwnerComp.GetAIOwner()->GetPawn());
    if (AICharacter == nullptr)
    {
        return EBTNodeResult::Failed;
    }

    AICharacter->Shoot();

    return EBTNodeResult::Succeeded;
}

