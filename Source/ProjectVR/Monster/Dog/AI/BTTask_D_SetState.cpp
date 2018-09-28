// Fill out your copyright notice in the Description page of Project Settings.

#include "BTTask_D_SetState.h"
#include "Monster/Dog/DogAIController.h"
#include "Monster/Dog/Dog.h"
#include "BehaviorTree/BlackboardComponent.h"							// 커스텀 대기시간

EBTNodeResult::Type UBTTask_D_SetState::ExecuteTask(UBehaviorTreeComponent & OwnerComp, uint8 * NodeMemory)
{
	ADogAIController* AI = Cast<ADogAIController>(OwnerComp.GetAIOwner());

	if (AI)
	{
		ADog* Dog = Cast<ADog>(AI->GetPawn());

		if (Dog)
		{
			Dog->CurrentDogState = DogState;
			Dog->CurrentDogAnimState = DogAnimState;
			Dog->CurrentDogBattleState = DogBattleState;
			Dog->CurrentDogCircleState = DogCircleState;
			Dog->CurrentDogJumpState = DogJumpState;
			Dog->CurrentDogAirState = DogAirState;
		}
	}

	return EBTNodeResult::Succeeded;
}
