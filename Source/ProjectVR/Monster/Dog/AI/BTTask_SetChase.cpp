// Fill out your copyright notice in the Description page of Project Settings.

#include "BTTask_SetChase.h"

#include "Monster/Dog/Dog.h"
#include "Monster/Dog/DogAIController.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "MyCharacter/MotionControllerCharacter.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Camera/CameraComponent.h"

EBTNodeResult::Type UBTTask_SetChase::ExecuteTask(UBehaviorTreeComponent & OwnerComp, uint8 * NodeMemory)
{
	ADogAIController* AI = Cast<ADogAIController>(OwnerComp.GetAIOwner());

	if (AI)
	{
		ADog* Dog = Cast<ADog>(AI->GetPawn());
		AActor* Player = Cast<AActor>(AI->BBComponent->GetValueAsObject(TEXT("Player")));
		AMotionControllerCharacter* MyCharacter = Cast<AMotionControllerCharacter>(Player);

		Distance = FVector::Distance(Dog->GetActorLocation(), MyCharacter->Camera->GetComponentLocation());

		if (Dog)
		{
			if (Distance > 400.0f)
			{
				Dog->CurrentDogState = EDogState::Chase;
				Dog->CurrentDogAnimState = EDogAnimState::Run;
				Dog->CurrentDogJumpState = EDogJumpState::Nothing;
				Dog->CurrentDogCircleState = EDogCircleState::Nothing;
			}
			else
			{
				Dog->CurrentDogState = EDogState::Battle;
				Dog->CurrentDogAnimState = EDogAnimState::Nothing;
				Dog->CurrentDogJumpState = EDogJumpState::Nothing;
				Dog->CurrentDogCircleState = EDogCircleState::Nothing;
			}
		}
	}
	return EBTNodeResult::Succeeded;
}
