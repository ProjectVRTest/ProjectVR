// Fill out your copyright notice in the Description page of Project Settings.

#include "BTTask_SetJumpStart.h"

#include "Monster/Dog/Dog.h"
#include "Monster/Dog/DogAIController.h"

EBTNodeResult::Type UBTTask_SetJumpStart::ExecuteTask(UBehaviorTreeComponent & OwnerComp, uint8 * NodeMemory)
{
	ADogAIController* AI = Cast<ADogAIController>(OwnerComp.GetAIOwner());

	if (AI)
	{
		ADog* Dog = Cast<ADog>(AI->GetPawn());

		if (Dog)
		{

			if (Dog->CurrentDogAnimState == EDogAnimState::JumpAttack)
			{
				if (Dog->CurrentDogJumpState == EDogJumpState::Nothing)
				{
					//UE_LOG(LogClass, Warning, TEXT("aSIAN GAME"));
					Dog->bIsAttack = true;
					Dog->CurrentDogJumpState = EDogJumpState::JumpStart;
				}
			}
		}
	}
	return EBTNodeResult::Succeeded;
}
