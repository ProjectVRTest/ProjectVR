// Fill out your copyright notice in the Description page of Project Settings.

#include "BTService_DistanceCalculation.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "MyCharacter/MotionControllerCharacter.h"
#include "Monster/Dog/Dog.h"
#include "Monster/Dog/DogAIController.h"
#include "Camera/CameraComponent.h"

void UBTService_DistanceCalculation::InitializeFromAsset(UBehaviorTree & Asset)
{
	Super::InitializeFromAsset(Asset);

	Distance = 0.0f;
}

void UBTService_DistanceCalculation::TickNode(UBehaviorTreeComponent & OwnerComp, uint8 * NodeMemory, float DeltaSeconds)
{
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);

	ADogAIController* AI = Cast<ADogAIController>(OwnerComp.GetAIOwner());

	if (AI)
	{
		AActor* Player = Cast<AActor>(AI->BBComponent->GetValueAsObject(TEXT("Player")));

		if (Player)
		{
			AMotionControllerCharacter* MyCharacter = Cast<AMotionControllerCharacter>(Player);
			ADog* RagdollDog = Cast<ADog>(AI->GetPawn());

			if (RagdollDog && MyCharacter)
			{

				Distance = FVector::Distance(RagdollDog->GetActorLocation(), MyCharacter->Camera->GetComponentLocation());

				if ( Distance < 400.0f)
				{
					RagdollDog->CurrentDogState = EDogState::Battle;
					RagdollDog->CurrentDogBattleState = EDogBattleState::Circle;

				}
			}
		}
	}
}
