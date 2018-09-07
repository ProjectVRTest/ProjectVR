// Fill out your copyright notice in the Description page of Project Settings.

#include "BTTask_PatrolState.h"
#include "Headers/NormalMonsterAIHeader.h"

EBTNodeResult::Type UBTTask_PatrolState::ExecuteTask(UBehaviorTreeComponent & OwnerComp, uint8 * NodeMemory)
{
	ANormalMonsterAIController* AI = Cast<ANormalMonsterAIController>(OwnerComp.GetAIOwner());

	if (AI)
	{
		ANormalMonster* NormalMonster = Cast<ANormalMonster>(AI->GetPawn());

		if (NormalMonster)
		{
			PatrolPoints = NormalMonster->PatrolPoints;

			int PatrolPointsLenth = PatrolPoints.Num();

			if (PatrolPointsLenth == 1)
			{
				NormalMonster->CurrentState = ENormalMonsterState::Idle;
				NormalMonster->CurrentAnimState = ENormalMonsterAnimState::Wait;
			}
			else
			{
				int RandomPatrolPoint = FMath::RandRange(0, PatrolPointsLenth-1);
				PatrolPoint = PatrolPoints[RandomPatrolPoint];
				AI->BBComponent->SetValueAsObject("PatrolPoint", PatrolPoint);
			}		
		}
	}
	
	return EBTNodeResult::Succeeded;
}
