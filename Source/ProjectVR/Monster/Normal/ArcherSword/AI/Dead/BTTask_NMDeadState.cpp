// Fill out your copyright notice in the Description page of Project Settings.

#include "BTTask_NMDeadState.h"
#include "Headers/NormalMonsterAIHeader.h"
#include "Components/SkeletalMeshComponent.h"

void UBTTask_NMDeadState::InitializeFromAsset(UBehaviorTree & Asset)
{
	Super::InitializeFromAsset(Asset);
	
	bNotifyTick = true;	
}

EBTNodeResult::Type UBTTask_NMDeadState::ExecuteTask(UBehaviorTreeComponent & OwnerComp, uint8 * NodeMemory)
{
	Super::ExecuteTask(OwnerComp, NodeMemory);

	DeathMaterialsValue = 0;

	AI = Cast<ANormalMonsterAIController>(OwnerComp.GetAIOwner());

	if (AI)
	{
		NormalMonster = Cast<ANormalMonster>(AI->GetPawn());
			
		if (NormalMonster)
		{						
			GetWorld()->GetTimerManager().SetTimer(DestroyRenderTimer, this, &UBTTask_NMDeadState::DestroyRender, 0.02f,true);
		}
	}

	return EBTNodeResult::InProgress;
}

void UBTTask_NMDeadState::Destroy()
{
	if (AI)
	{
		GetWorld()->GetTimerManager().ClearTimer(DestroyTimer);
		AI->GetPawn()->Destroy();
	}
}

void UBTTask_NMDeadState::DestroyRender()
{
	if (NormalMonster)
	{
		DeathMaterialsValue += 0.01;
		NormalMonster->GetMesh()->SetScalarParameterValueOnMaterials(TEXT("Amount"), DeathMaterialsValue);

		if (DeathMaterialsValue >= 1.0f)
		{
			GetWorld()->GetTimerManager().SetTimer(DestroyTimer, this, &UBTTask_NMDeadState::Destroy, 0.1f, false);			
			GetWorld()->GetTimerManager().ClearTimer(DestroyRenderTimer);
		}
	}
}