// Fill out your copyright notice in the Description page of Project Settings.

#include "BTTask_RevocationLanding.h"
#include "Monster/Dog/Dog.h"
#include "Monster/Dog/DogAIController.h"

EBTNodeResult::Type UBTTask_RevocationLanding::ExecuteTask(UBehaviorTreeComponent & OwnerComp, uint8 * NodeMemory)
{
	ADogAIController* AI = Cast<ADogAIController>(OwnerComp.GetAIOwner());

	if (AI)
	{
		ADog* Dog = Cast<ADog>(AI->GetPawn());

		if (Dog)
		{
			Dog->Landing = false;
			UE_LOG(LogTemp, Log, TEXT("eeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeee"));
			return EBTNodeResult::Succeeded;
		}
	}
	return EBTNodeResult::Failed;
}
