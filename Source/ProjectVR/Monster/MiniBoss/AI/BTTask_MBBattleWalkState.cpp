// Fill out your copyright notice in the Description page of Project Settings.

#include "BTTask_MBBattleWalkState.h"
#include "Headers/MiniBossAIHeader.h"

void UBTTask_MBBattleWalkState::InitializeFromAsset(UBehaviorTree & Asset)
{
	Super::InitializeFromAsset(Asset);

	bNotifyTick = true;
	RandomMove = 0;
}

EBTNodeResult::Type UBTTask_MBBattleWalkState::ExecuteTask(UBehaviorTreeComponent & OwnerComp, uint8 * NodeMemory)
{
	AI = Cast<AMiniBossAIController>(OwnerComp.GetAIOwner());

	if (AI)
	{
		MiniBoss = Cast<AMiniBoss>(AI->GetPawn());

		RandomMove = FMath::RandBool();
		AI->BBComponent->SetValueAsInt("RandomMove", RandomMove);
	}

	return EBTNodeResult::InProgress;
}

void UBTTask_MBBattleWalkState::TickTask(UBehaviorTreeComponent & OwnerComp, uint8 * NodeMemory, float DeltaSeconds)
{
	Super::TickTask(OwnerComp, NodeMemory, DeltaSeconds);


	if (MiniBoss)
	{
		switch (RandomMove)
		{
		case 0:
			MiniBoss->AddMovementInput(MiniBoss->GetActorRightVector(), 1.5f);
			MiniBoss->AddMovementInput(MiniBoss->GetActorForwardVector(), 1.5f);
			break;
		case 1:
			MiniBoss->AddMovementInput(MiniBoss->GetActorRightVector(), -1.5f);
			MiniBoss->AddMovementInput(MiniBoss->GetActorForwardVector(), 1.5f);
			break;
		}		
	}
}
