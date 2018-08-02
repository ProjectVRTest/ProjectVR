// Fill out your copyright notice in the Description page of Project Settings.

#include "BTTask_JumpAttackState.h"

#include "Monster/MiniBoss/MiniBossAIController.h"
#include "Monster/MiniBoss/MiniBoss.h"

#include "GameFramework/CharacterMovementComponent.h"
#include "Components/CapsuleComponent.h"

void UBTTask_JumpAttackState::InitializeFromAsset(UBehaviorTree & Asset)
{
	Super::InitializeFromAsset(Asset);

	bNotifyTick = true;
	CurrentFalling = false;
	PreviousFalling = false;

}

EBTNodeResult::Type UBTTask_JumpAttackState::ExecuteTask(UBehaviorTreeComponent & OwnerComp, uint8 * NodeMemory)
{
	AMiniBossAIController* AI = Cast<AMiniBossAIController>(OwnerComp.GetAIOwner());

	if (AI)
	{
		MiniBoss = Cast<AMiniBoss>(AI->GetPawn());
	}
	return EBTNodeResult::InProgress;
}

void UBTTask_JumpAttackState::TickTask(UBehaviorTreeComponent & OwnerComp, uint8 * NodeMemory, float DeltaSeconds)
{
	Super::TickTask(OwnerComp, NodeMemory, DeltaSeconds);

	if (MiniBoss)
	{
		FFindFloorResult FloorDistance;
		CurrentFalling = MiniBoss->CurrentFalling;

		if (CurrentFalling == true && PreviousFalling == true)
		{
			MiniBoss->CurrentJumpState = EMonsterJumpState::JumpLoop;
		}

		if (CurrentFalling == false && PreviousFalling == true)
		{
			MiniBoss->GetCharacterMovement()->ComputeFloorDist(MiniBoss->GetCapsuleComponent()->GetComponentLocation(), 10000.0f, 10000.0f, FloorDistance, 34.0f);

			if (FloorDistance.FloorDist < 250.0f)
			{
				MiniBoss->CurrentJumpState = EMonsterJumpState::JumpLand;
				MiniBoss->JumpRunCheckFlag = true;
				FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
			}
		}

		PreviousFalling = CurrentFalling;
	}
}
