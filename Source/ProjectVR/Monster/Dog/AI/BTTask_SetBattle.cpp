// Fill out your copyright notice in the Description page of Project Settings.

#include "BTTask_SetBattle.h"
#include "Monster/Dog/Dog.h"
#include "Monster/Dog/DogAIController.h"


EBTNodeResult::Type UBTTask_SetBattle::ExecuteTask(UBehaviorTreeComponent & OwnerComp, uint8 * NodeMemory)
{
	ADogAIController* AI = Cast<ADogAIController>(OwnerComp.GetAIOwner());

	if (AI)
	{
		ADog* Dog = Cast<ADog>(AI->GetPawn());

		if (Dog)
		{
			Dog->CurrentDogState = EDogState::Battle;
			Dog->CurrentDogAnimState = EDogAnimState::Nothing;
			Dog->CurrentDogJumpState = EDogJumpState::Nothing;
			Dog->CurrentDogCircleState = EDogCircleState::Nothing;
			return EBTNodeResult::Succeeded;
		}
	}
	return EBTNodeResult::Failed;
}
