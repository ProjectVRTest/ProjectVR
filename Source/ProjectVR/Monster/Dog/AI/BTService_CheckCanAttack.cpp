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

	Range = 50.0f;
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
			float StandardAngle = MyCharacter->Camera->GetComponentRotation().Yaw + 180.0f;		// �÷��̾� ���� ����
			float MonAngle = RagdollDog->GetActorRotation().Yaw + 180.0f;	// �� ���� ����

			float Max = StandardAngle + Range;		// ���� ���ݰ��� �� ���� 
			float Min = Max - (Range * 2);			// ���� ���ݰ��� �� ����

			// 0~360�� ���� -> 360�� �ʰ��� 0������ ����
			Max = Max >= 360.0f ? Max - 360.0f : Max;
			Min = Min < 0.0f ? 360.0f + Min : Min;

			Max += 180.0;		// �� ���� - �÷��̾� ������ �� �������� �ٲ�
			Min += 180.0;		// �� ���� - �÷��̾� ������ �� �������� �ٲ�

			// 0~360�� ���� -> 360�� �ʰ��� 0������ ����
			Max = Max >= 360.0f ? Max - 360.0f : Max;
			Min = Min >= 360.0f ? Min - 360.0f : Min;

			// ������ ���� ����� ���� 6����, ���� ���� ������ �÷��̾�� ���� ���� ���� -------------------------------------------------------------------- �ּ� ���
			if (StandardAngle <= Range && StandardAngle >= 0.0f)
			{
				if (MonAngle >= StandardAngle && MonAngle < Min)
				{
					AI->BBComponent->SetValueAsInt("RotateCheck", 1);
					RagdollDog->CurrentDogState = EDogState::Circle;
					RagdollDog->CurrentDogAnimState = EDogAnimState::LeftSideWalk;
					//RagdollDog->CurrentDogAnimState = EDogAnimState::LeftSideWalk;
				}
				else if (MonAngle < StandardAngle || MonAngle > Max)
				{
					AI->BBComponent->SetValueAsInt("RotateCheck", 2);
					RagdollDog->CurrentDogState = EDogState::Circle;
					RagdollDog->CurrentDogAnimState = EDogAnimState::RightSideWalk;
					//RagdollDog->CurrentDogAnimState = EDogAnimState::RightSideWalk;
				}
				else
				{
					RagdollDog->CurrentDogState = EDogState::Battle;
					RagdollDog->CurrentDogAnimState = EDogAnimState::JumpAttack;

					if (!RagdollDog->bIsAttack)
						RagdollDog->CurrentDogJumpState = EDogJumpState::Nothing;		// SetJumpStart���� JumpStart�� �ڵ� ����

					RagdollDog->GetCharacterMovement()->MaxWalkSpeed = 550.0f;
				}
			}
			else if (StandardAngle >= 360.0f - Range && StandardAngle >= 0.0f)		
			{
				if (MonAngle >= StandardAngle || MonAngle < Min)
				{
					AI->BBComponent->SetValueAsInt("RotateCheck", 1);
					RagdollDog->CurrentDogState = EDogState::Circle;
					RagdollDog->CurrentDogAnimState = EDogAnimState::LeftSideWalk;
				}
				else if (MonAngle < StandardAngle && MonAngle > Max)
				{
					AI->BBComponent->SetValueAsInt("RotateCheck", 2);
					RagdollDog->CurrentDogState = EDogState::Circle;
					RagdollDog->CurrentDogAnimState = EDogAnimState::RightSideWalk;
				}
				else
				{
					RagdollDog->CurrentDogState = EDogState::Battle;
					RagdollDog->CurrentDogAnimState = EDogAnimState::JumpAttack;

					if (!RagdollDog->bIsAttack)
						RagdollDog->CurrentDogJumpState = EDogJumpState::Nothing;		// SetJumpStart���� JumpStart�� �ڵ� ����

					RagdollDog->GetCharacterMovement()->MaxWalkSpeed = 550.0f;
				}

			}
			else if (StandardAngle <= 180.0f + Range && StandardAngle >= 180.0f)	// 3
			{
				if (MonAngle >= StandardAngle && MonAngle < Min)
				{
					AI->BBComponent->SetValueAsInt("RotateCheck", 1);
					RagdollDog->CurrentDogState = EDogState::Circle;
					RagdollDog->CurrentDogAnimState = EDogAnimState::LeftSideWalk;
				}
				else if (MonAngle < StandardAngle && MonAngle > Max)
				{
					AI->BBComponent->SetValueAsInt("RotateCheck", 2);
					RagdollDog->CurrentDogState = EDogState::Circle;
					RagdollDog->CurrentDogAnimState = EDogAnimState::RightSideWalk;
				}
				else
				{
					RagdollDog->CurrentDogState = EDogState::Battle;
					RagdollDog->CurrentDogAnimState = EDogAnimState::JumpAttack;

					if (!RagdollDog->bIsAttack)
						RagdollDog->CurrentDogJumpState = EDogJumpState::Nothing;		// SetJumpStart���� JumpStart�� �ڵ� ����

					RagdollDog->GetCharacterMovement()->MaxWalkSpeed = 550.0f;
				}
			}
			else if (StandardAngle >= 180.0f - Range && StandardAngle <= 180.0f)	// 4
			{
				if (MonAngle >= StandardAngle && MonAngle < Min)
				{
					AI->BBComponent->SetValueAsInt("RotateCheck", 1);
					RagdollDog->CurrentDogState = EDogState::Circle;
					RagdollDog->CurrentDogAnimState = EDogAnimState::LeftSideWalk;
				}
				else if (MonAngle < StandardAngle && MonAngle > Max)
				{
					AI->BBComponent->SetValueAsInt("RotateCheck", 2);
					RagdollDog->CurrentDogState = EDogState::Circle;
					RagdollDog->CurrentDogAnimState = EDogAnimState::RightSideWalk;
				}
				else
				{
					RagdollDog->CurrentDogState = EDogState::Battle;
					RagdollDog->CurrentDogAnimState = EDogAnimState::JumpAttack;

					if (!RagdollDog->bIsAttack)
						RagdollDog->CurrentDogJumpState = EDogJumpState::Nothing;		// SetJumpStart���� JumpStart�� �ڵ� ����

					RagdollDog->GetCharacterMovement()->MaxWalkSpeed = 550.0f;
				}
			}
			else if (StandardAngle < 360.0f - Range && StandardAngle > 180.0f + Range)	// 5
			{
				if (MonAngle >= StandardAngle || MonAngle < Min)
				{
					AI->BBComponent->SetValueAsInt("RotateCheck", 1);
					RagdollDog->CurrentDogState = EDogState::Circle;
					RagdollDog->CurrentDogAnimState = EDogAnimState::LeftSideWalk;
				}
				else if (MonAngle < StandardAngle && MonAngle > Max)
				{
					AI->BBComponent->SetValueAsInt("RotateCheck", 2);
					RagdollDog->CurrentDogState = EDogState::Circle;
					RagdollDog->CurrentDogAnimState = EDogAnimState::RightSideWalk;
				}
				else
				{
					RagdollDog->CurrentDogState = EDogState::Battle;
					RagdollDog->CurrentDogAnimState = EDogAnimState::JumpAttack;

					if (!RagdollDog->bIsAttack)
						RagdollDog->CurrentDogJumpState = EDogJumpState::Nothing;		// SetJumpStart���� JumpStart�� �ڵ� ����

					RagdollDog->GetCharacterMovement()->MaxWalkSpeed = 550.0f;
				}
			}
			else if (StandardAngle > 0.0f + Range && StandardAngle < 180.0f - Range)	// 6
			{
				if (MonAngle >= StandardAngle && MonAngle < Min)
				{
					AI->BBComponent->SetValueAsInt("RotateCheck", 1);
					RagdollDog->CurrentDogState = EDogState::Circle;
					RagdollDog->CurrentDogAnimState = EDogAnimState::LeftSideWalk;
				}
				else if (MonAngle < StandardAngle || MonAngle > Max)
				{
					AI->BBComponent->SetValueAsInt("RotateCheck", 2);
					RagdollDog->CurrentDogState = EDogState::Circle;
					RagdollDog->CurrentDogAnimState = EDogAnimState::RightSideWalk;
				}
				else
				{
					RagdollDog->CurrentDogState = EDogState::Battle;
					RagdollDog->CurrentDogAnimState = EDogAnimState::JumpAttack;

					if (!RagdollDog->bIsAttack)
						RagdollDog->CurrentDogJumpState = EDogJumpState::Nothing;		// SetJumpStart���� JumpStart�� �ڵ� ����

					RagdollDog->GetCharacterMovement()->MaxWalkSpeed = 550.0f;
				}
			}
			else
			{
				RagdollDog->CurrentDogState = EDogState::Battle;
				RagdollDog->CurrentDogAnimState = EDogAnimState::JumpAttack;

				if (!RagdollDog->bIsAttack)
					RagdollDog->CurrentDogJumpState = EDogJumpState::Nothing;		// SetJumpStart���� JumpStart�� �ڵ� ����

				RagdollDog->GetCharacterMovement()->MaxWalkSpeed = 550.0f;
			}
		}
	}
	
}
