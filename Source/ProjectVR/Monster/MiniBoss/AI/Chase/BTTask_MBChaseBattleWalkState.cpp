// Fill out your copyright notice in the Description page of Project Settings.

#include "BTTask_MBChaseBattleWalkState.h"
#include "Headers/MiniBossAIHeader.h"

#define LEFT 1
#define RIGHT 2

void UBTTask_MBChaseBattleWalkState::InitializeFromAsset(UBehaviorTree & Asset)
{
	Super::InitializeFromAsset(Asset);

	bNotifyTick = true;
	RandomMove = 0;
	MoveCount = 0;
	RandomChaseValue = 0;
}

EBTNodeResult::Type UBTTask_MBChaseBattleWalkState::ExecuteTask(UBehaviorTreeComponent & OwnerComp, uint8 * NodeMemory)
{
	AI = Cast<AMiniBossAIController>(OwnerComp.GetAIOwner());

	if (AI)
	{
		MiniBoss = Cast<AMiniBoss>(AI->GetPawn());

		RandomMove = FMath::RandRange(1, 2);
		RandomChaseValue = FMath::RandRange(30, 60);
		AI->BBComponent->SetValueAsInt("RandomMove", RandomMove);
	}

	return EBTNodeResult::InProgress;
}

void UBTTask_MBChaseBattleWalkState::TickTask(UBehaviorTreeComponent & OwnerComp, uint8 * NodeMemory, float DeltaSeconds)
{
	Super::TickTask(OwnerComp, NodeMemory, DeltaSeconds);

	MoveCount++;
	
	if (MiniBoss)
	{
		switch (RandomMove)
		{
		case RIGHT:
			MiniBoss->AddMovementInput(MiniBoss->GetActorRightVector(), 1.5f);
			MiniBoss->AddMovementInput(MiniBoss->GetActorForwardVector(), 1.5f);	
			break;
		case LEFT:
			MiniBoss->AddMovementInput(MiniBoss->GetActorRightVector(), -1.5f);
			MiniBoss->AddMovementInput(MiniBoss->GetActorForwardVector(), 1.5f);	
			break;
		}

		if (MoveCount >= RandomChaseValue)
		{
			MoveCount = 0;
			RandomChaseValue = FMath::RandRange(30, 60);
			RandomMove = FMath::RandRange(1, 2);
		}
	}
}
