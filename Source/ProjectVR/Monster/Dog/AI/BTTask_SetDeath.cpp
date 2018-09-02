// Fill out your copyright notice in the Description page of Project Settings.

#include "BTTask_SetDeath.h"
#include "Monster/Dog/Dog.h"
#include "Monster/Dog/DogAIController.h"
#include "MyCharacter/MotionControllerCharacter.h"
#include "BehaviorTree/BlackboardComponent.h"

// 캐릭터 도그배열에 있을 수 있으니까 제거해야함

EBTNodeResult::Type UBTTask_SetDeath::ExecuteTask(UBehaviorTreeComponent & OwnerComp, uint8 * NodeMemory)
{
	ADogAIController* AI = Cast<ADogAIController>(OwnerComp.GetAIOwner());
	AActor* Player = Cast<AActor>(AI->BBComponent->GetValueAsObject(TEXT("Player")));
	AMotionControllerCharacter* MyCharacter = Cast<AMotionControllerCharacter>(Player);

	if (AI && MyCharacter)
	{
		ADog* Dog = Cast<ADog>(AI->GetPawn());

		if (Dog)
		{
			if (MyCharacter->DogArray.Contains(Dog))
				MyCharacter->DogArray.Remove(Dog);

			Dog->CurrentDogState = EDogState::Death;
			Dog->CurrentDogAnimState = EDogAnimState::Nothing;
			Dog->CurrentDogJumpState = EDogJumpState::Nothing;
			Dog->CurrentDogCircleState = EDogCircleState::Nothing;

			return EBTNodeResult::Succeeded;
		}
	}
	return EBTNodeResult::Failed;
}
