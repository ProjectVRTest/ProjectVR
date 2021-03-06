// Fill out your copyright notice in the Description page of Project Settings.

#include "BTTask_NMIdleState.h"
#include "Headers/NormalMonsterAIHeader.h"

void UBTTask_NMIdleState::InitializeFromAsset(UBehaviorTree & Asset)
{
	Super::InitializeFromAsset(Asset);
}

EBTNodeResult::Type UBTTask_NMIdleState::ExecuteTask(UBehaviorTreeComponent & OwnerComp, uint8 * NodeMemory)
{
	Super::ExecuteTask(OwnerComp, NodeMemory);

	ANormalMonsterAIController* AI = Cast<ANormalMonsterAIController>(OwnerComp.GetAIOwner());

	if (AI)
	{
		NormalMonster = Cast<ANormalMonster>(AI->GetPawn());
		
		if (NormalMonster->CurrentIdleState == ENormalMonsterIdleState::Wait)
		{
			GetWorld()->GetTimerManager().SetTimer(PatrolOnTimer, this, &UBTTask_NMIdleState::PatrolStart, 0.5f, false);
		}
		else
		{
			return EBTNodeResult::Succeeded;
		}		
	}
	return EBTNodeResult::InProgress;
}

void UBTTask_NMIdleState::PatrolStart()
{
	if (NormalMonster)
	{		
		if (NormalMonster->CurrentState == ENormalMonsterState::AttackWait)
		{
			return;
		}
		else
		{
			NormalMonster->CurrentState = ENormalMonsterState::Patrol;
			NormalMonster->CurrentAnimState = ENormalMonsterAnimState::Walk;
		}		
	}
}
