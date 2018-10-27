// Fill out your copyright notice in the Description page of Project Settings.

#include "BTTask_BossBWDistanceCheck.h"
#include "Headers/BossAIHeader.h"

EBTNodeResult::Type UBTTask_BossBWDistanceCheck::ExecuteTask(UBehaviorTreeComponent & OwnerComp, uint8 * NodeMemory)
{
	ABossAIController* AI = Cast<ABossAIController>(OwnerComp.GetAIOwner());

	if (AI)
	{
		ABoss* Boss = Cast<ABoss>(AI->GetPawn());

		if (Boss)
		{
			float Distance = AI->BBComponent->GetValueAsFloat("Distance");
			if (Distance >= 400.0f)
			{
				//인비저블 시작
			}
			else
			{
				Boss->CurrentBattleWatchState = EBossBattleWatchState::Idle;
				Boss->CurrentBattleState = EBossBattleState::AttackReady;
			}
		}
		else
		{
			GLog->Log(FString::Printf(TEXT("보스가 없음")));
		}
	}
	return EBTNodeResult::Succeeded;
}
