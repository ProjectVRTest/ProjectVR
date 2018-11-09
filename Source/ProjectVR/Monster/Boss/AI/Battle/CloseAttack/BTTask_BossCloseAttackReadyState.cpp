// Fill out your copyright notice in the Description page of Project Settings.

#include "BTTask_BossCloseAttackReadyState.h"
#include "Headers/BossAIHeader.h"

EBTNodeResult::Type UBTTask_BossCloseAttackReadyState::ExecuteTask(UBehaviorTreeComponent & OwnerComp, uint8 * NodeMemory)
{
	ABossAIController* AI = Cast<ABossAIController>(OwnerComp.GetAIOwner());

	if (AI)
	{
		ABoss* Boss = Cast<ABoss>(AI->GetPawn());

		int RandValue = 0;

		if (Boss)
		{
			RandValue = FMath::RandRange(1, 15);
			
			//1~5 패링이 가능한 공격
			if (RandValue >= 1 && RandValue <= 5)
			{
				Boss->CurrentCloseAttackState = EBossCloseAttackState::RightUpLeftDownAttack;
			}// 6~11 콤보 공격
			else if (RandValue >= 6 && RandValue <= 11)
			{
				Boss->CurrentCloseAttackState = EBossCloseAttackState::ComboAttack;
			} //위에서 아래로 찍는 공격
			else if (RandValue >= 12)
			{
				Boss->CurrentUpDownAttackState = EBossUpDownAttackState::UpDownAttackStart;
				Boss->CurrentCloseAttackState = EBossCloseAttackState::UpDownAttack;
			}

		}
	}

	return EBTNodeResult::Succeeded;
}
