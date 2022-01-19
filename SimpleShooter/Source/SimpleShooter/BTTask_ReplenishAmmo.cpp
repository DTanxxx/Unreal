// Fill out your copyright notice in the Description page of Project Settings.


#include "BTTask_ReplenishAmmo.h"
#include "SimpleShooter/ShooterCharacter.h"
#include "AIController.h"

UBTTask_ReplenishAmmo::UBTTask_ReplenishAmmo()
{
    NodeName = TEXT("Replenish Ammo");
}

EBTNodeResult::Type UBTTask_ReplenishAmmo::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
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

    AICharacter->AddAmmoForGunIndex(1, AICharacter->GetActiveGunIndex());

    return EBTNodeResult::Succeeded;
}
