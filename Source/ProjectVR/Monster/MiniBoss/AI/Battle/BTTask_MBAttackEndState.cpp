// Fill out your copyright notice in the Description page of Project Settings.

#include "BTTask_MBAttackEndState.h"
#include "Headers/MiniBossAIHeader.h"
#include "MyCharacter/CameraLocation.h"
#include "Components/SkeletalMeshComponent.h"

EBTNodeResult::Type UBTTask_MBAttackEndState::ExecuteTask(UBehaviorTreeComponent & OwnerComp, uint8 * NodeMemory)
{
	AMiniBossAIController* AI = Cast<AMiniBossAIController>(OwnerComp.GetAIOwner());

	if (AI)
	{
		AMiniBoss* MiniBoss = Cast<AMiniBoss>(AI->GetPawn());	
		AI->BBComponent->SetValueAsFloat(TEXT("AttackAnimationWaitTime"), 0);
		AI->BBComponent->SetValueAsFloat(TEXT("ReverseWaitTime"), 0);
		float Distance = AI->BBComponent->GetValueAsFloat("Distance");

		if (MiniBoss)
		{
			ACameraLocation* CameraLocation = Cast<ACameraLocation>(AI->BBComponent->GetValueAsObject("PlayerCamera"));

			FRotator LookAt = UKismetMathLibrary::NormalizedDeltaRotator(MiniBoss->GetActorRotation(), UKismetMathLibrary::FindLookAtRotation(MiniBoss->GetActorLocation(), CameraLocation->GetActorLocation()));

			//float BackAttackYawValue = UKismetMathLibrary::Abs(LookAt.Yaw);
			

			if (-180.0f < LookAt.Yaw && LookAt.Yaw < -150.0f)
			{
				if (Distance < 300.0f)
				{
					MiniBoss->CurrentState = EMiniBossState::Battle;
					MiniBoss->CurrentAnimState = EMiniBossAnimState::Attack;
					MiniBoss->CurrentJumpState = EMiniBossJumpState::Idle;
					MiniBoss->CurrentAttackState = EMiniBossAttackState::BackAttack;
					MiniBoss->CurrentShortAttackState = EMiniBossShortAttackState::Idle;
					MiniBoss->CurrentWaveAttackState = EMiniBossWaveAttackState::Idle;
					MiniBoss->CurrentComboAttackState = EMiniBossComboAttackState::Idle;
					MiniBoss->CurrentBackAttackState = EMiniBossBackAttackState::LeftBackAttackStart;
				}
				else
				{
					MiniBoss->CurrentState = EMiniBossState::Battle;
					MiniBoss->CurrentAnimState = EMiniBossAnimState::Rotate;
					MiniBoss->CurrentJumpState = EMiniBossJumpState::Idle;
					MiniBoss->CurrentAttackState = EMiniBossAttackState::Idle;
					MiniBoss->CurrentShortAttackState = EMiniBossShortAttackState::Idle;
					MiniBoss->CurrentWaveAttackState = EMiniBossWaveAttackState::Idle;
					MiniBoss->CurrentComboAttackState = EMiniBossComboAttackState::Idle;
					MiniBoss->CurrentBackAttackState = EMiniBossBackAttackState::Idle;
				}
			}
			else if (150.0f < LookAt.Yaw && LookAt.Yaw < 180.0f)
			{
				if (Distance < 300.0f)
				{
					MiniBoss->CurrentState = EMiniBossState::Battle;
					MiniBoss->CurrentAnimState = EMiniBossAnimState::Attack;
					MiniBoss->CurrentJumpState = EMiniBossJumpState::Idle;
					MiniBoss->CurrentAttackState = EMiniBossAttackState::BackAttack;
					MiniBoss->CurrentShortAttackState = EMiniBossShortAttackState::Idle;
					MiniBoss->CurrentWaveAttackState = EMiniBossWaveAttackState::Idle;
					MiniBoss->CurrentComboAttackState = EMiniBossComboAttackState::Idle;
					MiniBoss->CurrentBackAttackState = EMiniBossBackAttackState::RightBackAttackStart;
				}
				else
				{
					MiniBoss->CurrentState = EMiniBossState::Battle;
					MiniBoss->CurrentAnimState = EMiniBossAnimState::Rotate;
					MiniBoss->CurrentJumpState = EMiniBossJumpState::Idle;
					MiniBoss->CurrentAttackState = EMiniBossAttackState::Idle;
					MiniBoss->CurrentShortAttackState = EMiniBossShortAttackState::Idle;
					MiniBoss->CurrentWaveAttackState = EMiniBossWaveAttackState::Idle;
					MiniBoss->CurrentComboAttackState = EMiniBossComboAttackState::Idle;
					MiniBoss->CurrentBackAttackState = EMiniBossBackAttackState::Idle;
				}
			}		
			else if (-50.0f < LookAt.Yaw && LookAt.Yaw < 50.0f)
			{
				MiniBoss->CurrentState = EMiniBossState::Battle;
				MiniBoss->CurrentAnimState = EMiniBossAnimState::AttackReady;
				MiniBoss->CurrentJumpState = EMiniBossJumpState::Idle;
				MiniBoss->CurrentAttackState = EMiniBossAttackState::Idle;
				MiniBoss->CurrentShortAttackState = EMiniBossShortAttackState::Idle;
				MiniBoss->CurrentWaveAttackState = EMiniBossWaveAttackState::Idle;
				MiniBoss->CurrentComboAttackState = EMiniBossComboAttackState::Idle;
				MiniBoss->CurrentBackAttackState = EMiniBossBackAttackState::Idle;
			}
			else
			{
				MiniBoss->CurrentState = EMiniBossState::Battle;
				MiniBoss->CurrentAnimState = EMiniBossAnimState::Rotate;
				MiniBoss->CurrentJumpState = EMiniBossJumpState::Idle;
				MiniBoss->CurrentAttackState = EMiniBossAttackState::Idle;
				MiniBoss->CurrentShortAttackState = EMiniBossShortAttackState::Idle;
				MiniBoss->CurrentWaveAttackState = EMiniBossWaveAttackState::Idle;
				MiniBoss->CurrentComboAttackState = EMiniBossComboAttackState::Idle;
				MiniBoss->CurrentBackAttackState = EMiniBossBackAttackState::Idle;
			}
		}
	}

	return EBTNodeResult::Succeeded;
}
