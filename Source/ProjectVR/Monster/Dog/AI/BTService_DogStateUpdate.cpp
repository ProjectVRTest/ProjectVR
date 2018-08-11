// Fill out your copyright notice in the Description page of Project Settings.

#include "BTService_DogStateUpdate.h"

#include "BehaviorTree/BlackboardComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "MyCharacter/MotionControllerCharacter.h"
#include "Monster/Dog/Dog.h"
#include "Monster/Dog/DogAIController.h"
#include "Camera/CameraComponent.h"

void UBTService_DogStateUpdate::InitializeFromAsset(UBehaviorTree & Asset)
{
	Super::InitializeFromAsset(Asset);
	AttackDistance = 400.0f;
}

void UBTService_DogStateUpdate::TickNode(UBehaviorTreeComponent & OwnerComp, uint8 * NodeMemory, float DeltaSeconds)
{
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);

	ADogAIController* AI = Cast<ADogAIController>(OwnerComp.GetAIOwner());

	if (AI)
	{
		AActor* Player = Cast<AActor>(AI->BBComponent->GetValueAsObject(TEXT("Player")));
		AMotionControllerCharacter* MyCharacter = Cast<AMotionControllerCharacter>(Player);
		ADog* RagdollDog = Cast<ADog>(AI->GetPawn());
		
		if (RagdollDog && MyCharacter)
		{
			Distance = FVector::Distance(RagdollDog->GetActorLocation(), MyCharacter->Camera->GetComponentLocation());

			DistanceWithLand = AI->BBComponent->GetValueAsFloat("DistanceWithLand");
			//UE_LOG(LogClass, Warning, TEXT("%f"), Distance);

			switch (RagdollDog->CurrentDogState)
			{
			case EDogState::Idle:
				RagdollDog->GetCharacterMovement()->MaxWalkSpeed = 0.0f;
				break;
			case EDogState::Chase:
				RagdollDog->GetCharacterMovement()->MaxWalkSpeed = 200.0f;
				break;
			case EDogState::Battle:
				RagdollDog->GetCharacterMovement()->MaxWalkSpeed = 200.0f;

				if (Distance > 400.0f)
				{
					RagdollDog->CurrentDogState = EDogState::Chase;
					RagdollDog->CurrentDogAnimState = EDogAnimState::Run;
					RagdollDog->CurrentDogJumpState = EDogJumpState::Nothing;
					RagdollDog->CurrentDogCircleState = EDogCircleState::Nothing;
				}
				break;
			case EDogState::Hurled:
				RagdollDog->GetCharacterMovement()->MaxWalkSpeed = 0.0f;
				break;
			case EDogState::Death:
				RagdollDog->GetCharacterMovement()->MaxWalkSpeed = 0.0f;
				break;
			case EDogState::Nothing:
				RagdollDog->GetCharacterMovement()->MaxWalkSpeed = 0.0f;
				break;
			default:
				break;
			}
		}
	}
}
