// Fill out your copyright notice in the Description page of Project Settings.

#include "BTTask_MBRotateState.h"
#include "Headers/MiniBossAIHeader.h"

void UBTTask_MBRotateState::InitializeFromAsset(UBehaviorTree & Asset)
{
	Super::InitializeFromAsset(Asset);

	bNotifyTick = true;
}

EBTNodeResult::Type UBTTask_MBRotateState::ExecuteTask(UBehaviorTreeComponent & OwnerComp, uint8 * NodeMemory)
{
	AMiniBossAIController* AI = Cast<AMiniBossAIController>(OwnerComp.GetAIOwner());

	if (AI)
	{
		MiniBoss = Cast<AMiniBoss>(AI->GetPawn());

		if (MiniBoss)
		{
			MyCharacter = Cast<AMotionControllerCharacter>(AI->BBComponent->GetValueAsObject("Player"));

			if (MyCharacter)
			{
				LookAt = UKismetMathLibrary::FindLookAtRotation(MiniBoss->GetActorLocation(), MyCharacter->Camera->GetComponentLocation());
				AI->BBComponent->SetValueAsRotator("LookAtRotator", LookAt);
				MiniBoss->RotateYaw = LookAt.Yaw;
			}
		}		
	}

	return EBTNodeResult::InProgress;
}

void UBTTask_MBRotateState::TickTask(UBehaviorTreeComponent & OwnerComp, uint8 * NodeMemory, float DeltaSeconds)
{
	Super::TickTask(OwnerComp, NodeMemory, DeltaSeconds);

	if (MiniBoss)
	{
		FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
	}
}
