// Fill out your copyright notice in the Description page of Project Settings.

#include "BTTask_DogDeath.h"

#include "Monster/Dog/DogAIController.h"
#include "Monster/Dog/Dog.h"
#include "Components/SkeletalMeshComponent.h"


EBTNodeResult::Type UBTTask_DogDeath::ExecuteTask(UBehaviorTreeComponent & OwnerComp, uint8 * NodeMemory)
{
	Super::ExecuteTask(OwnerComp, NodeMemory);

	ADogAIController* AI = Cast<ADogAIController>(OwnerComp.GetAIOwner());

	if (AI)
	{
		AI->GetPawn()->Destroy();
		return EBTNodeResult::Succeeded;
	}
	return EBTNodeResult::Failed;
}
