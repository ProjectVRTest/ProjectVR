// Fill out your copyright notice in the Description page of Project Settings.

#include "BTService_DogStateUpdate.h"

#include "BehaviorTree/BlackboardComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "MyCharacter/MotionControllerCharacter.h"
#include "Monster/Dog/Dog.h"
#include "Monster/Dog/DogAIController.h"

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
			Distance = FVector::Distance(RagdollDog->GetActorLocation(), Player->GetActorLocation());

			DistanceWithLand = AI->BBComponent->GetValueAsFloat("DistanceWithLand");
			//UE_LOG(LogClass, Warning, TEXT("%f"), Distance);

			switch (RagdollDog->CurrentDogState)
			{
			case EDogState::Idle:

				break;
			case EDogState::Chase:
				if (Distance <= 400.0f)
				{
					RagdollDog->CurrentDogState = EDogState::Battle;

					RagdollDog->GetCharacterMovement()->MaxWalkSpeed = 0;
				}
				break;
			case EDogState::Battle:
				if (Distance > 400.0f)
				{
					RagdollDog->CurrentDogState = EDogState::Chase;
					RagdollDog->GetCharacterMovement()->MaxWalkSpeed = 550;
				}
				else if(DistanceWithLand < 3.0f)
				{
					RagdollDog->CurrentDogState = EDogState::Battle;
					//RagdollDog->CurrentDogAnimState = EDogAnimState::SideWalk;

					if (!RagdollDog->bIsAttack)
						RagdollDog->CurrentDogJumpState = EDogJumpState::Nothing;		// SetJumpStart에서 JumpStart로 자동 세팅

																						//RagdollDog->CurrentDogState = EDogState::Battle;
																						//RagdollDog->CurrentDogAnimState = EDogAnimState::JumpAttack;

																						//if(!RagdollDog->bIsAttack)
																						//	RagdollDog->CurrentDogJumpState = EDogJumpState::Nothing;		// SetJumpStart에서 JumpStart로 자동 세팅

																						//RagdollDog->GetCharacterMovement()->MaxWalkSpeed = 550;
				}
				break;
			case EDogState::Hurled:

				break;
			case EDogState::Death:

				break;
			case EDogState::Nothing:

				break;
			default:
				break;
			}
		}
	}
}
