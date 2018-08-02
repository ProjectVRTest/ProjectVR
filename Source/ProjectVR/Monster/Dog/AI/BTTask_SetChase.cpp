// Fill out your copyright notice in the Description page of Project Settings.

#include "BTTask_SetChase.h"

#include "Monster/Dog/Dog.h"
#include "Monster/Dog/DogAIController.h"
#include "GameFramework/CharacterMovementComponent.h"

EBTNodeResult::Type UBTTask_SetChase::ExecuteTask(UBehaviorTreeComponent & OwnerComp, uint8 * NodeMemory)
{
	ADogAIController* AI = Cast<ADogAIController>(OwnerComp.GetAIOwner());

	if (AI)
	{
		ADog* Dog = Cast<ADog>(AI->GetPawn());

		if (Dog)
		{
			Dog->CurrentDogState = EDogState::Chase;
			Dog->CurrentDogAnimState = EDogAnimState::Run;
			Dog->CurrentDogJumpState = EDogJumpState::Nothing;
			Dog->GetCharacterMovement()->MaxWalkSpeed = 550.0f;
		}
	}
	return EBTNodeResult::Succeeded;
}
