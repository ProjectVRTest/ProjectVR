// Fill out your copyright notice in the Description page of Project Settings.

#include "BTTask_D_B_J_SetBiteCollision.h"
#include "Monster/Dog/DogAIController.h"
#include "Monster/Dog/Dog.h"
#include "Components/BoxComponent.h"

EBTNodeResult::Type UBTTask_D_B_J_SetBiteCollision::ExecuteTask(UBehaviorTreeComponent & OwnerComp, uint8 * NodeMemory)
{
	ADogAIController* AI = Cast<ADogAIController>(OwnerComp.GetAIOwner());

	if (AI)
	{
		ADog* Dog = Cast<ADog>(AI->GetPawn());

		if (Dog)
		{
			Dog->DogAttackCollision->bGenerateOverlapEvents = check;
		}
		return EBTNodeResult::Succeeded;
	}
	return EBTNodeResult::Failed;
}
