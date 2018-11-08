// Fill out your copyright notice in the Description page of Project Settings.

#include "BTTask_BossUltimateAttackLoop.h"
#include "Headers/BossAIHeader.h"
#include "Monster/Boss/Orb/DefaultOrb/BossOrb.h"
#include "Particles/ParticleSystemComponent.h"

void UBTTask_BossUltimateAttackLoop::InitializeFromAsset(UBehaviorTree & Asset)
{
	Super::InitializeFromAsset(Asset);

	bNotifyTick = true;
}

EBTNodeResult::Type UBTTask_BossUltimateAttackLoop::ExecuteTask(UBehaviorTreeComponent & OwnerComp, uint8 * NodeMemory)
{
	ExitFlag = false;

	AI = Cast<ABossAIController>(OwnerComp.GetAIOwner());

	if (AI)
	{
		Boss = Cast<ABoss>(AI->GetPawn());

		GetWorld()->GetTimerManager().SetTimer(UltimateEndTimer, this, &UBTTask_BossUltimateAttackLoop::UltimateEnd, 20.0f, false);
	}

	return EBTNodeResult::InProgress;
}

void UBTTask_BossUltimateAttackLoop::TickTask(UBehaviorTreeComponent & OwnerComp, uint8 * NodeMemory, float DeltaSeconds)
{
	Super::TickTask(OwnerComp, NodeMemory, DeltaSeconds);

	if (Boss)
	{
		CurrentNormalMonsterCount = Boss->CurrentNormalMonsterCount;

		if (ExitFlag || CurrentNormalMonsterCount <=0)
		{
			if (CurrentNormalMonsterCount <= 0)
			{
				UltimateEnd();
			}

			GLog->Log(FString::Printf(TEXT("궁극기 시전 끝나서 End상태로 ㄱㄱ")));
			Boss->CurrentNormalMonsterCount = 3;
			Boss->UltimateAuraEffectComponent->SetVisibility(false);
			Boss->CurrentUltimateAttackState = EBossUltimateAttackState::UltimateAttackEnd;
			FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
		}
	}	
}

void UBTTask_BossUltimateAttackLoop::UltimateEnd()
{
	if (Boss->CurrentBattleState == EBossBattleState::UltimateAttack)
	{
		ExitFlag = true; //현재 상태를 탈출시켜 주기 위해 EndFlag를 켜준다.

		if (Boss->UltimateOrbs.Num() > 0)
		{
			for (auto Orb : Boss->UltimateOrbs)
			{
				if (Orb)
				{
					Orb->Destroy(); //남아있는 오브가 있으면 삭제 시킨다.
				}
			}

			Boss->UltimateOrbs.Empty(); //궁극기 오브를 담아둔 배열을 깨끗하게 정리한다.
		}
		GLog->Log(FString::Printf(TEXT("남아 있는 오브 수 %d"), Boss->UltimateOrbs.Num()));
	}
}
