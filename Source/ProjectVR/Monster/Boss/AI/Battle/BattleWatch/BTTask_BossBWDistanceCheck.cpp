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
			
			if (Distance >= 600.0f)
			{
				Boss->CurrentBattleWatchState = EBossBattleWatchState::InVisible;
			}
			else
			{
				int RandomPatternValue = FMath::RandRange(1, 10);
								
				if (RandomPatternValue < 5)
				{
					Boss->CurrentBattleWatchState = EBossBattleWatchState::InVisible;
				}
				else
				{
					Boss->CurrentConfrontationState = EBossConfrontationState::ConfrontationReady;
					Boss->CurrentBattleWatchState = EBossBattleWatchState::Confrontation;						
				}
			}
		}
		else
		{
			GLog->Log(FString::Printf(TEXT("보스가 없음")));
		}
	}
	return EBTNodeResult::Succeeded;
}
