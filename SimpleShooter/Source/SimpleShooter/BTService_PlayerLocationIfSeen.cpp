// Fill out your copyright notice in the Description page of Project Settings.


#include "BTService_PlayerLocationIfSeen.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/Pawn.h"
#include "AIController.h"
#include "SimpleShooter/ShooterCharacter.h"

UBTService_PlayerLocationIfSeen::UBTService_PlayerLocationIfSeen()
{
    NodeName = TEXT("Update Player Location If Seen");
}

void UBTService_PlayerLocationIfSeen::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
    Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);

    AShooterCharacter* Player = Cast<AShooterCharacter>(UGameplayStatics::GetPlayerPawn(GetWorld(), 0));
    if (Player == nullptr)
    {
        return;
    }

    AAIController* AIOwner = OwnerComp.GetAIOwner();
    if (AIOwner == nullptr)
    {
        return;
    }

    // if player is in line of sight of AI, set focus and move to player
    if (AIOwner->LineOfSightTo(Player))
    {
        // Set PlayerLocation
        OwnerComp.GetBlackboardComponent()->SetValueAsObject(GetSelectedBlackboardKey(), Player);

        // SetFocus(PlayerPawn);
        // MoveToActor(PlayerPawn, AcceptanceRadius);
    }
    else
    {
        // Clear PlayerLocation
        OwnerComp.GetBlackboardComponent()->ClearValue(GetSelectedBlackboardKey());

        // ClearFocus(EAIFocusPriority::Gameplay);
        // StopMovement();
    }
}
