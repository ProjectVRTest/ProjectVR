// Fill out your copyright notice in the Description page of Project Settings.

#include "BTTask_DisappearActor.h"

#include "Object/NavigationActor/NavigationActor.h"

#include "Object/NavigationActor/NavigationAIController.h"
EBTNodeResult::Type UBTTask_DisappearActor::ExecuteTask(UBehaviorTreeComponent & OwnerComp, uint8 * NodeMemory)
{
	Super::ExecuteTask(OwnerComp, NodeMemory);

	ANavigationAIController* AI = Cast<ANavigationAIController>(OwnerComp.GetAIOwner());

	if (AI)
	{
		AI->GetPawn()->Destroy();
		return EBTNodeResult::Succeeded;
	}

	return EBTNodeResult::Failed;
}
