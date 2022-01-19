// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BTTask_ReplenishAmmo.generated.h"

UCLASS()
class SIMPLESHOOTER_API UBTTask_ReplenishAmmo : public UBTTaskNode
{
	GENERATED_BODY()

public:
	UBTTask_ReplenishAmmo();

protected:
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
	
};
