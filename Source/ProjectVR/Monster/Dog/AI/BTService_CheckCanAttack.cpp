// Fill out your copyright notice in the Description page of Project Settings.

#include "BTService_CheckCanAttack.h"
#include "Monster/Dog/Dog.h"
#include "Monster/Dog/DogAIController.h"
#include "MyCharacter/MotionControllerCharacter.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "Camera/CameraComponent.h"


void UBTService_CheckCanAttack::InitializeFromAsset(UBehaviorTree & Asset)
{
	Super::InitializeFromAsset(Asset);

	Range = 40.0f;
}

void UBTService_CheckCanAttack::TickNode(UBehaviorTreeComponent & OwnerComp, uint8 * NodeMemory, float DeltaSeconds)
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
			float StandardAngle = MyCharacter->Camera->GetComponentRotation().Yaw + 180.0f;
			float MonAngle = RagdollDog->GetActorRotation().Yaw + 180.0f;


			float Max = StandardAngle + Range;
			float Min = Max - (Range * 2);

			Max = Max >= 360.0f ? Max - 360.0f : Max;
			Min = Min < 0.0f ? 360.0f + Min : Min;

			Max += 180.0;
			Min += 180.0;

			Max = Max >= 360.0f ? Max - 360.0f : Max;
			Min = Min >= 360.0f ? Min - 360.0f : Min;

			float oppPlayer = StandardAngle + 180.0f;
			oppPlayer = oppPlayer >= 360.0f ? oppPlayer - 360.0f : oppPlayer;

			if (StandardAngle <= Range && StandardAngle >= 0.0f)		// 1
			{
				if (MonAngle >= StandardAngle && MonAngle < Min)
				{
					AI->BBComponent->SetValueAsInt("RotateCheck", 1);
					RagdollDog->CurrentDogAnimState = EDogAnimState::LeftSideWalk;
					//UE_LOG(LogClass, Warning, TEXT("Left ------ %f / %f / %f / %f"), StandardAngle, Min, Max, MonAngle);

				}
				else if (MonAngle < StandardAngle || MonAngle > Max)
				{
					AI->BBComponent->SetValueAsInt("RotateCheck", 2);
					RagdollDog->CurrentDogAnimState = EDogAnimState::RightSideWalk;
					//UE_LOG(LogClass, Warning, TEXT("Right ------ %f / %f / %f / %f"), StandardAngle, Min, Max, MonAngle);
				}
				else
				{
					RagdollDog->CurrentDogState = EDogState::Battle;
					RagdollDog->CurrentDogAnimState = EDogAnimState::JumpAttack;

					if (!RagdollDog->bIsAttack)
						RagdollDog->CurrentDogJumpState = EDogJumpState::Nothing;		// SetJumpStart에서 JumpStart로 자동 세팅

					RagdollDog->GetCharacterMovement()->MaxWalkSpeed = 550;
				}
			}
			else if (StandardAngle >= 360.0f - Range && StandardAngle >= 0.0f)		// 2
			{
				if (MonAngle >= StandardAngle || MonAngle < Min)
				{
					AI->BBComponent->SetValueAsInt("RotateCheck", 1);
					//UE_LOG(LogClass, Warning, TEXT("Left ------ %f / %f / %f / %f"), StandardAngle, Min, Max, MonAngle);


				}
				else if (MonAngle < StandardAngle && MonAngle > Max)
				{
					AI->BBComponent->SetValueAsInt("RotateCheck", 2);
					//UE_LOG(LogClass, Warning, TEXT("Right ------ %f / %f / %f / %f"), StandardAngle, Min, Max, MonAngle);
				}
				else
				{
					RagdollDog->CurrentDogState = EDogState::Battle;
					RagdollDog->CurrentDogAnimState = EDogAnimState::JumpAttack;

					if (!RagdollDog->bIsAttack)
						RagdollDog->CurrentDogJumpState = EDogJumpState::Nothing;		// SetJumpStart에서 JumpStart로 자동 세팅

					RagdollDog->GetCharacterMovement()->MaxWalkSpeed = 550;
				}

			}
			else if (StandardAngle <= 180.0f + Range && StandardAngle >= 180.0f)	// 3
			{
				if (MonAngle >= StandardAngle && MonAngle < Min)
				{
					AI->BBComponent->SetValueAsInt("RotateCheck", 1);
					//UE_LOG(LogClass, Warning, TEXT("Left ------ %f / %f / %f / %f"), StandardAngle, Min, Max, MonAngle);


				}
				else if (MonAngle < StandardAngle && MonAngle > Max)
				{
					AI->BBComponent->SetValueAsInt("RotateCheck", 2);
					//UE_LOG(LogClass, Warning, TEXT("Right ------ %f / %f / %f / %f"), StandardAngle, Min, Max, MonAngle);
				}
				else
				{
					RagdollDog->CurrentDogState = EDogState::Battle;
					RagdollDog->CurrentDogAnimState = EDogAnimState::JumpAttack;

					if (!RagdollDog->bIsAttack)
						RagdollDog->CurrentDogJumpState = EDogJumpState::Nothing;		// SetJumpStart에서 JumpStart로 자동 세팅

					RagdollDog->GetCharacterMovement()->MaxWalkSpeed = 550;
				}
			}
			else if (StandardAngle >= 180.0f - Range && StandardAngle <= 180.0f)	// 4
			{
				if (MonAngle >= StandardAngle && MonAngle < Min)
				{
					AI->BBComponent->SetValueAsInt("RotateCheck", 1);
					//UE_LOG(LogClass, Warning, TEXT("Left ------ %f / %f / %f / %f"), StandardAngle, Min, Max, MonAngle);


				}
				else if (MonAngle < StandardAngle && MonAngle > Max)
				{
					AI->BBComponent->SetValueAsInt("RotateCheck", 2);
					//UE_LOG(LogClass, Warning, TEXT("Right ------ %f / %f / %f / %f"), StandardAngle, Min, Max, MonAngle);
				}
				else
				{
					RagdollDog->CurrentDogState = EDogState::Battle;
					RagdollDog->CurrentDogAnimState = EDogAnimState::JumpAttack;

					if (!RagdollDog->bIsAttack)
						RagdollDog->CurrentDogJumpState = EDogJumpState::Nothing;		// SetJumpStart에서 JumpStart로 자동 세팅

					RagdollDog->GetCharacterMovement()->MaxWalkSpeed = 550;
				}
			}
			else if (StandardAngle < 360.0f - Range && StandardAngle > 180.0f + Range)	// 5
			{
				if (MonAngle >= StandardAngle || MonAngle < Min)
				{
					AI->BBComponent->SetValueAsInt("RotateCheck", 1);
					//UE_LOG(LogClass, Warning, TEXT("Left ------ %f / %f / %f / %f"), StandardAngle, Min, Max, MonAngle);


				}
				else if (MonAngle < StandardAngle && MonAngle > Max)
				{
					AI->BBComponent->SetValueAsInt("RotateCheck", 2);
					//UE_LOG(LogClass, Warning, TEXT("Right ------ %f / %f / %f / %f"), StandardAngle, Min, Max, MonAngle);
				}
				else
				{
					RagdollDog->CurrentDogState = EDogState::Battle;
					RagdollDog->CurrentDogAnimState = EDogAnimState::JumpAttack;

					if (!RagdollDog->bIsAttack)
						RagdollDog->CurrentDogJumpState = EDogJumpState::Nothing;		// SetJumpStart에서 JumpStart로 자동 세팅

					RagdollDog->GetCharacterMovement()->MaxWalkSpeed = 550;
				}
			}
			else if (StandardAngle > 0.0f + Range && StandardAngle < 180.0f - Range)	// 6
			{
				if (MonAngle >= StandardAngle && MonAngle < Min)
				{
					AI->BBComponent->SetValueAsInt("RotateCheck", 1);
					//UE_LOG(LogClass, Warning, TEXT("Left ------ %f / %f / %f / %f"), StandardAngle, Min, Max, MonAngle);


				}
				else if (MonAngle < StandardAngle || MonAngle > Max)
				{
					AI->BBComponent->SetValueAsInt("RotateCheck", 2);
					//UE_LOG(LogClass, Warning, TEXT("Right ------ %f / %f / %f / %f"), StandardAngle, Min, Max, MonAngle);
				}
				else
				{
					RagdollDog->CurrentDogState = EDogState::Battle;
					RagdollDog->CurrentDogAnimState = EDogAnimState::JumpAttack;

					if (!RagdollDog->bIsAttack)
						RagdollDog->CurrentDogJumpState = EDogJumpState::Nothing;		// SetJumpStart에서 JumpStart로 자동 세팅

					RagdollDog->GetCharacterMovement()->MaxWalkSpeed = 550;
				}
			}

			//UE_LOG(LogClass, Warning, TEXT("%f / %f / %f / %f"), StandardAngle, Max, Min, MonAngle);



		}
	}
	
}
