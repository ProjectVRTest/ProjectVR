// Fill out your copyright notice in the Description page of Project Settings.

#include "BTTask_BossBattleWatchIdleState.h"
#include "Headers/BossAIHeader.h"

EBTNodeResult::Type UBTTask_BossBattleWatchIdleState::ExecuteTask(UBehaviorTreeComponent & OwnerComp, uint8 * NodeMemory)
{
	ABossAIController* AI = Cast<ABossAIController>(OwnerComp.GetAIOwner());

	if (AI)
	{
		Boss = Cast<ABoss>(AI->GetPawn());

		if (Boss)
		{
			float Distance = AI->BBComponent->GetValueAsFloat("Distance");

			if (Distance < 600.0f)
			{
				GetWorld()->GetTimerManager().SetTimer(BattleWatchTimer, this, &UBTTask_BossBattleWatchIdleState::BattleWatchWalk, 3.0f, false);
				Boss->CurrentBattleWatchState = EBossBattleWatchState::Walk;
			}
			else
			{
				Boss->CurrentBattleWatchState = EBossBattleWatchState::Idle;
				Boss->CurrentBattleState = EBossBattleState::AttackReady;
			}
		}
	}
	return EBTNodeResult::Succeeded;
}

void UBTTask_BossBattleWatchIdleState::BattleWatchWalk()
{
	if (Boss)
	{
		if (Boss->CurrentBattleState == EBossBattleState::BattleWatch)
		{
			GLog->Log(FString::Printf(TEXT("주시 상태임")));
			Boss->CurrentBattleWatchState = EBossBattleWatchState::BattleWatchDistanceCheck;
		}
	}
	else
	{
		GLog->Log(FString::Printf(TEXT("보스가 없음")));
	}
	GLog->Log(FString::Printf(TEXT("배틀워치워크 타이머 발동")));
}
