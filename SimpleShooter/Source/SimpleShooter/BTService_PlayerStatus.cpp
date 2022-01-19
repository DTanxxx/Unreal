// Fill out your copyright notice in the Description page of Project Settings.


#include "BTService_PlayerStatus.h"
#include "Kismet/GameplayStatics.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "GameFramework/Pawn.h"
#include "SimpleShooter/ShooterCharacter.h"

UBTService_PlayerStatus::UBTService_PlayerStatus()
{
    NodeName = TEXT("Remove Player Location and Last Location Seen If Dead");
}

void UBTService_PlayerStatus::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
    Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);
    
    // Remove Player and LastKnownPlayerLocation keys (via hardcoded strings) if player dead
    AShooterCharacter* Player = Cast<AShooterCharacter>(UGameplayStatics::GetPlayerPawn(GetWorld(), 0));
    if (Player == nullptr)
    {
        // Player Died
        OwnerComp.GetBlackboardComponent()->ClearValue(FName(TEXT("Player")));
        OwnerComp.GetBlackboardComponent()->ClearValue(FName(TEXT("LastKnownPlayerLocation")));
        return;
    }
}
