// Fill out your copyright notice in the Description page of Project Settings.

#include "BTTask_JumpReadyState.h"

#include "Monster/MiniBoss/MiniBossAIController.h"
#include "Monster/MiniBoss/MiniBoss.h"

#include "BehaviorTree/BlackboardComponent.h"

#include "Engine/World.h"
#include "TimerManager.h"


EBTNodeResult::Type UBTTask_JumpReadyState::ExecuteTask(UBehaviorTreeComponent & OwnerComp, uint8 * NodeMemory)
{
	AMiniBossAIController* AI = Cast<AMiniBossAIController>(OwnerComp.GetAIOwner());

	if (AI)
	{
		MiniBoss = Cast<AMiniBoss>(AI->GetPawn());

		if (MiniBoss)
		{
			float Distance = AI->BBComponent->GetValueAsFloat("Distance");

			int jumpRunValue = FMath::RandRange(1, 10);

			if (jumpRunValue <= 3)
			{
				MiniBoss->CurrentAnimState = EMonsterAnimState::JumpAttack;
				MiniBoss->CurrentJumpState = EMonsterJumpState::JumpStart;
			}
			else
			{
				MiniBoss->CurrentAnimState = EMonsterAnimState::Run;
				MiniBoss->CurrentJumpState = EMonsterJumpState::Idle;
			}

			if (MiniBoss->CurrentAnimState == EMonsterAnimState::Run)
			{
				GetWorld()->GetTimerManager().SetTimer(RunTimer, this, &UBTTask_JumpReadyState::JumpRunCheck, 5.0f, false);
			}			
		}
	}
	return EBTNodeResult::Succeeded;
}

void UBTTask_JumpReadyState::JumpRunCheck()
{
	if (MiniBoss)
	{
		MiniBoss->JumpRunCheckFlag = true;
	}
}
