// Fill out your copyright notice in the Description page of Project Settings.

#include "BTTask_MBJumpState.h"
#include "Headers/MiniBossAIHeader.h"

#include "Components/CapsuleComponent.h"
void UBTTask_MBJumpState::InitializeFromAsset(UBehaviorTree & Asset)
{
	Super::InitializeFromAsset(Asset);
	
	bNotifyTick = true;
	CurrentFalling = false;
	PreviousFalling = false;
}

EBTNodeResult::Type UBTTask_MBJumpState::ExecuteTask(UBehaviorTreeComponent & OwnerComp, uint8 * NodeMemory)
{
	AMiniBossAIController* AI = Cast<AMiniBossAIController>(OwnerComp.GetAIOwner());

	if (AI)
	{
		MiniBoss = Cast<AMiniBoss>(AI->GetPawn());
	}
	return EBTNodeResult::InProgress;
}

void UBTTask_MBJumpState::TickTask(UBehaviorTreeComponent & OwnerComp, uint8 * NodeMemory, float DeltaSeconds)
{
	Super::TickTask(OwnerComp, NodeMemory, DeltaSeconds);

	if (MiniBoss)
	{
		FFindFloorResult FloorDistance;
		CurrentFalling = MiniBoss->CurrentFalling;

		if (CurrentFalling && PreviousFalling)
		{
			MiniBoss->CurrentJumpState = EMiniBossJumpState::JumpLoop;
		}

		if (!CurrentFalling && PreviousFalling)
		{
			MiniBoss->GetCharacterMovement()->ComputeFloorDist(MiniBoss->GetCapsuleComponent()->GetComponentLocation(), 10000.0f, 10000.0f, FloorDistance, 34.0f);

			if (FloorDistance.FloorDist < 250.0f)
			{
				GLog->Log(FString::Printf(TEXT("asl")));
				MiniBoss->CurrentJumpState = EMiniBossJumpState::JumpLand;
				MiniBoss->JumpRunCheckFlag = false;
				MiniBoss->IsAttack = false; //다시 공격할 수 있게 해줌
				FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
			}
		}

		PreviousFalling = CurrentFalling;
	}
}
