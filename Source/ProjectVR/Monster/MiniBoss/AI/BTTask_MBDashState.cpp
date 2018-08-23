// Fill out your copyright notice in the Description page of Project Settings.

#include "BTTask_MBDashState.h"
#include "Headers/MiniBossAIHeader.h"

#include "Components/CapsuleComponent.h"
void UBTTask_MBDashState::InitializeFromAsset(UBehaviorTree & Asset)
{
	Super::InitializeFromAsset(Asset);
	
	bNotifyTick = true;
	CurrentFalling = false;
	PreviousFalling = false;
}

EBTNodeResult::Type UBTTask_MBDashState::ExecuteTask(UBehaviorTreeComponent & OwnerComp, uint8 * NodeMemory)
{
	AMiniBossAIController* AI = Cast<AMiniBossAIController>(OwnerComp.GetAIOwner());

	if (AI)
	{
		MiniBoss = Cast<AMiniBoss>(AI->GetPawn());

		if (MiniBoss)
		{
			MiniBoss->CurrentDashState = EMiniBossDashState::DashStart;
		}
	}

	return EBTNodeResult::InProgress;
}

void UBTTask_MBDashState::TickTask(UBehaviorTreeComponent & OwnerComp, uint8 * NodeMemory, float DeltaSeconds)
{
	Super::TickTask(OwnerComp, NodeMemory, DeltaSeconds);

	if (MiniBoss)
	{
		FFindFloorResult FloorDistance;
		CurrentFalling = MiniBoss->CurrentFalling;

		if (CurrentFalling && PreviousFalling)
		{
			GLog->Log(FString::Printf(TEXT("DashLoop")));
			MiniBoss->CurrentDashState = EMiniBossDashState::DashLoop;
		}

		if (!CurrentFalling && PreviousFalling)
		{
			MiniBoss->GetCharacterMovement()->ComputeFloorDist(MiniBoss->GetCapsuleComponent()->GetComponentLocation(), 10000.0f, 10000.0f, FloorDistance, 34.0f);

			if (FloorDistance.FloorDist < 100.0f)
			{
				GLog->Log(FString::Printf(TEXT("DashEnd")));
				MiniBoss->CurrentDashState = EMiniBossDashState::DashEnd;
				MiniBoss->IsAttack = false;
				FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
			}
		}
		PreviousFalling = CurrentFalling;
	}
}
