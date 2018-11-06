// Fill out your copyright notice in the Description page of Project Settings.

#include "BTTask_NMDMArcherDeadState.h"
#include "Headers/DontMoveArcherAIHeader.h"
#include "Components/SkeletalMeshComponent.h"

void UBTTask_NMDMArcherDeadState::InitializeFromAsset(UBehaviorTree & Asset)
{
	Super::InitializeFromAsset(Asset);

	bNotifyTick = true;
}

EBTNodeResult::Type UBTTask_NMDMArcherDeadState::ExecuteTask(UBehaviorTreeComponent & OwnerComp, uint8 * NodeMemory)
{
	AI = Cast<ADontMoveArcherAIController>(OwnerComp.GetAIOwner());

	DeadInVisibleValue = 0;

	if (AI)
	{
		DontMoveArcher = Cast<ADontMoveArcher>(AI->GetPawn());
	}
	return EBTNodeResult::InProgress;
}

void UBTTask_NMDMArcherDeadState::TickTask(UBehaviorTreeComponent & OwnerComp, uint8 * NodeMemory, float DeltaSeconds)
{
	Super::TickTask(OwnerComp, NodeMemory, DeltaSeconds);

	if (AI)
	{
		if (DontMoveArcher)
		{
			if (DeadInVisibleValue > 1.0f)
			{
				DontMoveArcher->Destroy();
				FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
			}
			else
			{
				DeadInVisibleValue += 0.01;
				DontMoveArcher->GetMesh()->SetScalarParameterValueOnMaterials(TEXT("Amount"), DeadInVisibleValue);
			}
		}
	}
}

