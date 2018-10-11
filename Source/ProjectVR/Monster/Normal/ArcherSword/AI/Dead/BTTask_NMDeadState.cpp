// Fill out your copyright notice in the Description page of Project Settings.

#include "BTTask_NMDeadState.h"
#include "Headers/NormalMonsterAIHeader.h"

#include "Components/SkeletalMeshComponent.h"

void UBTTask_NMDeadState::InitializeFromAsset(UBehaviorTree & Asset)
{
	Super::InitializeFromAsset(Asset);

	DeathMaterialsValue = 0;
}

EBTNodeResult::Type UBTTask_NMDeadState::ExecuteTask(UBehaviorTreeComponent & OwnerComp, uint8 * NodeMemory)
{
	Super::ExecuteTask(OwnerComp, NodeMemory);

	AI = Cast<ANormalMonsterAIController>(OwnerComp.GetAIOwner());

	if (AI)
	{
		ANormalMonster*	NormalMonster = Cast<ANormalMonster>(AI->GetPawn());

		if (NormalMonster)
		{
			DeathMaterialsValue += 0.01;

			NormalMonster->GetMesh()->SetScalarParameterValueOnMaterials(TEXT("Amount"), DeathMaterialsValue);
			GetWorld()->GetTimerManager().SetTimer(DestroyTimer, this, &UBTTask_NMDeadState::Destroy, 3.0f, false);
		}
	}

	return EBTNodeResult::InProgress;
}

void UBTTask_NMDeadState::Destroy()
{
	if (AI)
	{
		AI->GetPawn()->Destroy();
	}
}