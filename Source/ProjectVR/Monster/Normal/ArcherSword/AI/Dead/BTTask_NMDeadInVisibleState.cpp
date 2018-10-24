// Fill out your copyright notice in the Description page of Project Settings.

#include "BTTask_NMDeadInVisibleState.h"
#include "Headers/NormalMonsterAIHeader.h"
#include "Components/SkeletalMeshComponent.h"

void UBTTask_NMDeadInVisibleState::InitializeFromAsset(UBehaviorTree & Asset)
{
	Super::InitializeFromAsset(Asset);

	bNotifyTick = true;
}

EBTNodeResult::Type UBTTask_NMDeadInVisibleState::ExecuteTask(UBehaviorTreeComponent & OwnerComp, uint8 * NodeMemory)
{
	AI = Cast<ANormalMonsterAIController>(OwnerComp.GetAIOwner());
	DeadInVisibleValue = 0;
	if (AI)
	{
		NormalMonster = Cast<ANormalMonster>(AI->GetPawn());
	}
	return EBTNodeResult::InProgress;
}

void UBTTask_NMDeadInVisibleState::TickTask(UBehaviorTreeComponent & OwnerComp, uint8 * NodeMemory, float DeltaSeconds)
{
	Super::TickTask(OwnerComp, NodeMemory, DeltaSeconds);

	if (AI)
	{
		if (NormalMonster)
		{
			DeadInVisibleValue += 0.01;
			NormalMonster->GetMesh()->SetScalarParameterValueOnMaterials(TEXT("Amount"), DeadInVisibleValue);

			if (DeadInVisibleValue >= 1.0f)
			{
				AI->GetPawn()->Destroy();
			}
		}
	}
}
