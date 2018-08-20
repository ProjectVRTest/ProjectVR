// Fill out your copyright notice in the Description page of Project Settings.

#include "BTTask_MBAttackReadyState.h"
#include "Headers/MiniBossAIHeader.h"


EBTNodeResult::Type UBTTask_MBAttackReadyState::ExecuteTask(UBehaviorTreeComponent & OwnerComp, uint8 * NodeMemory)
{
	AI = Cast<AMiniBossAIController>(OwnerComp.GetAIOwner());

	if (AI)
	{
		MiniBoss = Cast<AMiniBoss>(AI->GetPawn());

		if (MiniBoss)
		{
			int RandomAttackValue = FMath::RandRange(1, 3);

			MiniBoss->CurrentAttackState = EMiniBossAttackState::RightUpLeftDown;
			/*switch (RandomAttackValue)
			{
			case RightUpLeftDownAttack:
				MiniBoss->CurrentAttackState = EMiniBossAttackState::RightUpLeftDown;
				break;
			case TwoHandWidthAttack:
				MiniBoss->CurrentAttackState = EMiniBossAttackState::TwoHandWidth;
				break;
			case StabAttack:
				MiniBoss->CurrentAttackState = EMiniBossAttackState::Stab;
				break;
			}*/
		}

	}
	return EBTNodeResult::Type();
}

