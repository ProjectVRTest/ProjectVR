// Fill out your copyright notice in the Description page of Project Settings.

#include "BTTask_MBJumpAttackReadyState.h"
#include "Headers/MiniBossAIHeader.h"

EBTNodeResult::Type UBTTask_MBJumpAttackReadyState::ExecuteTask(UBehaviorTreeComponent & OwnerComp, uint8 * NodeMemory)
{
	AMiniBossAIController* AI = Cast<AMiniBossAIController>(OwnerComp.GetAIOwner());

	if (AI)
	{
		MiniBoss = Cast<AMiniBoss>(AI->GetPawn());

		if (MiniBoss)
		{
			float Distance = AI->BBComponent->GetValueAsFloat("Distance");

			if (Distance > 1500.0f)
			{
				//GLog->Log(FString::Printf(TEXT("거리가 1500 이상으로 바로 점프 시작")));
				MiniBoss->CurrentAnimState = EMiniBossAnimState::JumpAttack;
				MiniBoss->CurrentJumpState = EMiniBossJumpState::JumpStart;
			}
			else
			{
				int JumpWalkRandom = FMath::RandRange(1, 10);

				if (JumpWalkRandom <= 3)
				{
				//	GLog->Log(FString::Printf(TEXT("점프 공격 시작")));
					MiniBoss->CurrentAnimState = EMiniBossAnimState::JumpAttack;
					MiniBoss->CurrentJumpState = EMiniBossJumpState::JumpStart;
				}
				else
				{
				//	GLog->Log(FString::Printf(TEXT("다시 추격 시작")));
					MiniBoss->CurrentAnimState = EMiniBossAnimState::Walk;
					MiniBoss->CurrentJumpState = EMiniBossJumpState::Idle;
					
				}
			}
		}
	}

	return EBTNodeResult::Succeeded;
}

void UBTTask_MBJumpAttackReadyState::JumpRunCheck()
{
	if (MiniBoss)
	{
		if (MiniBoss->CurrentState == EMiniBossState::Chase)
		{
			if (MiniBoss->CurrentAnimState == EMiniBossAnimState::Walk)
			{
				MiniBoss->CurrentAnimState = EMiniBossAnimState::DashAttack;
			}
			else
			{

			}
		}
	}
}

