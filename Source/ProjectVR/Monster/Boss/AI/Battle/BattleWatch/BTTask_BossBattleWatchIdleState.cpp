// Fill out your copyright notice in the Description page of Project Settings.

#include "BTTask_BossBattleWatchIdleState.h"
#include "Headers/BossAIHeader.h"

EBTNodeResult::Type UBTTask_BossBattleWatchIdleState::ExecuteTask(UBehaviorTreeComponent & OwnerComp, uint8 * NodeMemory)
{
	//배틀 주시 상태
	ABossAIController* AI = Cast<ABossAIController>(OwnerComp.GetAIOwner()); //보스를 조정하고 있는 AI컨트롤러를 가져오고

	if (AI) //있으면
	{
		Boss = Cast<ABoss>(AI->GetPawn()); //보스를 가져오고

		if (Boss) //보스가 있으면
		{
			float Distance = AI->BBComponent->GetValueAsFloat("Distance"); //거리를 가져온다.

			if (Distance < 800.0f) //거리가 8M 아래로 가까워지면
			{
				//3초후에 전투 주시 거리 판단 상태로 바꿔주고
				GetWorld()->GetTimerManager().SetTimer(BattleWatchTimer, this, &UBTTask_BossBattleWatchIdleState::BattleWatchWalk, 3.0f, false);
				//캐릭터를 향해 걷는다.
				Boss->CurrentBattleWatchState = EBossBattleWatchState::Walk;
			}
			else
			{				
				//오브 or 원거리 공격을 할 수 있는 상태로 바꿔준다.
				Boss->CurrentBattleWatchState = EBossBattleWatchState::Idle;
				Boss->CurrentBattleState = EBossBattleState::AttackReady;
			}
		}
	}
	return EBTNodeResult::Succeeded;
}

void UBTTask_BossBattleWatchIdleState::BattleWatchWalk()
{
	//타이머가 발동되면
	//우선 보스가 존재하는지 확인하고
	if (Boss) //보스가 있고
	{
		//캐릭터의 전투상태가 전투주시 상태이면
		if (Boss->CurrentBattleState == EBossBattleState::BattleWatch)
		{
			GLog->Log(FString::Printf(TEXT("주시 상태임"))); 
			Boss->CurrentBattleWatchState = EBossBattleWatchState::BattleWatchDistanceCheck; //전투주시 거리 판단 상태로 바꿔준다.
		}
		//캐릭터의 전투상태가 전투주시 상태가 아니면 (전투걷기 상태에서 거리가 캐릭터와 좁혀져서 전투상태로 바뀐상태이면
		//상태값을 바꾸지 않는다.
	}
	else
	{
		GLog->Log(FString::Printf(TEXT("보스가 없음")));
	}
	GLog->Log(FString::Printf(TEXT("배틀워치워크 타이머 발동")));
}
