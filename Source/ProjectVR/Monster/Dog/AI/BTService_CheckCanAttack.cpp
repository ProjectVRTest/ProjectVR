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

	Range = 10.0f;
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

		if (RagdollDog->Landing)
			return;

		if (RagdollDog && MyCharacter)
		{
			float StandardAngle = MyCharacter->Camera->GetComponentRotation().Yaw + 180.0f;		// 플레이어 기준 각도
			float MonAngle = RagdollDog->GetActorRotation().Yaw + 180.0f;	// 개 기준 각도

			float Max = StandardAngle + Range;		// 개의 공격가능 우 범위 
			float Min = Max - (Range * 2);			// 개의 공격가능 좌 범위

			// 0~360도 범위 -> 360도 초과시 0도부터 시작
			Max = Max >= 360.0f ? Max - 360.0f : Max;
			Min = Min < 0.0f ? 360.0f + Min : Min;

			Max += 180.0;		// 우 범위 - 플레이어 기준을 개 기준으로 바꿈
			Min += 180.0;		// 좌 범위 - 플레이어 기준을 개 기준으로 바꿈

			// 0~360도 범위 -> 360도 초과시 0도부터 시작
			Max = Max >= 360.0f ? Max - 360.0f : Max;
			Min = Min >= 360.0f ? Min - 360.0f : Min;

			UE_LOG(LogTemp, Log, TEXT("%f / %f"), StandardAngle, Range);

			// 주위를 도는 경우의 수는 6가지, 개의 보는 각도랑 플레이어와 보는 각도 반전 -------------------------------------------------------------------- 주석 대기
			if (StandardAngle <= Range && StandardAngle >= 0.0f)
			{
				UE_LOG(LogTemp, Log, TEXT("1"));
				if (MonAngle >= StandardAngle && MonAngle < Min)
				{
					AI->BBComponent->SetValueAsInt("RotateCheck", 1);
					RagdollDog->CurrentDogAnimState = EDogAnimState::SideWalk;
					RagdollDog->CurrentDogCircleState = EDogCircleState::RightCircle;
				}
				else if (MonAngle < StandardAngle || MonAngle > Max)
				{
					AI->BBComponent->SetValueAsInt("RotateCheck", 2);
					RagdollDog->CurrentDogAnimState = EDogAnimState::SideWalk;
					RagdollDog->CurrentDogCircleState = EDogCircleState::LeftCircle;
				}
				else
				{
					RagdollDog->CurrentDogAnimState = EDogAnimState::JumpAttack;

					if (!RagdollDog->bIsAttack)
						RagdollDog->CurrentDogJumpState = EDogJumpState::Nothing;		// SetJumpStart에서 JumpStart로 자동 세팅

					RagdollDog->GetCharacterMovement()->MaxWalkSpeed = 550.0f;
				}
			}
			else if (StandardAngle >= 360.0f - Range && StandardAngle >= 0.0f)		
			{
				UE_LOG(LogTemp, Log, TEXT("2"));
				if (MonAngle >= StandardAngle || MonAngle < Min)
				{
					AI->BBComponent->SetValueAsInt("RotateCheck", 1);
					RagdollDog->CurrentDogAnimState = EDogAnimState::SideWalk;
					RagdollDog->CurrentDogCircleState = EDogCircleState::RightCircle;
				}
				else if (MonAngle < StandardAngle && MonAngle > Max)
				{
					AI->BBComponent->SetValueAsInt("RotateCheck", 2);
					RagdollDog->CurrentDogAnimState = EDogAnimState::SideWalk;
					RagdollDog->CurrentDogCircleState = EDogCircleState::LeftCircle;
				}
				else
				{
					RagdollDog->CurrentDogAnimState = EDogAnimState::JumpAttack;

					if (!RagdollDog->bIsAttack)
						RagdollDog->CurrentDogJumpState = EDogJumpState::Nothing;		// SetJumpStart에서 JumpStart로 자동 세팅

					RagdollDog->GetCharacterMovement()->MaxWalkSpeed = 550.0f;
				}

			}
			else if (StandardAngle <= 180.0f + Range && StandardAngle >= 180.0f)	// 3
			{
				UE_LOG(LogTemp, Log, TEXT("3"));
				if (MonAngle >= StandardAngle && MonAngle < Min)
				{
					AI->BBComponent->SetValueAsInt("RotateCheck", 1);
					RagdollDog->CurrentDogAnimState = EDogAnimState::SideWalk;
					RagdollDog->CurrentDogCircleState = EDogCircleState::RightCircle;
				}
				else if (MonAngle < StandardAngle && MonAngle > Max)
				{
					AI->BBComponent->SetValueAsInt("RotateCheck", 2);
					RagdollDog->CurrentDogAnimState = EDogAnimState::SideWalk;
					RagdollDog->CurrentDogCircleState = EDogCircleState::LeftCircle;
				}
				else
				{
					RagdollDog->CurrentDogAnimState = EDogAnimState::JumpAttack;

					if (!RagdollDog->bIsAttack)
						RagdollDog->CurrentDogJumpState = EDogJumpState::Nothing;		// SetJumpStart에서 JumpStart로 자동 세팅

					RagdollDog->GetCharacterMovement()->MaxWalkSpeed = 550.0f;
				}
			}
			else if (StandardAngle >= 180.0f - Range && StandardAngle <= 180.0f)	// 4
			{
				UE_LOG(LogTemp, Log, TEXT("4"));
				if (MonAngle >= StandardAngle && MonAngle < Min)
				{
					AI->BBComponent->SetValueAsInt("RotateCheck", 1);
					RagdollDog->CurrentDogAnimState = EDogAnimState::SideWalk;
					RagdollDog->CurrentDogCircleState = EDogCircleState::RightCircle;
				}
				else if (MonAngle < StandardAngle && MonAngle > Max)
				{
					AI->BBComponent->SetValueAsInt("RotateCheck", 2);
					RagdollDog->CurrentDogAnimState = EDogAnimState::SideWalk;
					RagdollDog->CurrentDogCircleState = EDogCircleState::LeftCircle;
				}
				else
				{
					RagdollDog->CurrentDogState = EDogState::Battle;
					RagdollDog->CurrentDogAnimState = EDogAnimState::JumpAttack;

					if (!RagdollDog->bIsAttack)
					{
						RagdollDog->CurrentDogJumpState = EDogJumpState::Nothing;		// SetJumpStart에서 JumpStart로 자동 세팅
					}
					RagdollDog->GetCharacterMovement()->MaxWalkSpeed = 550.0f;
				}
			}
			else if (StandardAngle < 360.0f - Range && StandardAngle > 180.0f + Range)	// 5
			{
				UE_LOG(LogTemp, Log, TEXT("5"));
				if (MonAngle >= StandardAngle || MonAngle < Min)
				{
					AI->BBComponent->SetValueAsInt("RotateCheck", 1);
					RagdollDog->CurrentDogAnimState = EDogAnimState::SideWalk;
					RagdollDog->CurrentDogCircleState = EDogCircleState::RightCircle;
				}
				else if (MonAngle < StandardAngle && MonAngle > Max)
				{
					AI->BBComponent->SetValueAsInt("RotateCheck", 2);
					RagdollDog->CurrentDogAnimState = EDogAnimState::SideWalk;
					RagdollDog->CurrentDogCircleState = EDogCircleState::LeftCircle;
				}
				else
				{
					RagdollDog->CurrentDogAnimState = EDogAnimState::JumpAttack;

					if (!RagdollDog->bIsAttack)
						RagdollDog->CurrentDogJumpState = EDogJumpState::Nothing;		// SetJumpStart에서 JumpStart로 자동 세팅

					RagdollDog->GetCharacterMovement()->MaxWalkSpeed = 550.0f;
				}
			}
			else if (StandardAngle > 0.0f + Range && StandardAngle < 180.0f - Range)	// 6
			{
				UE_LOG(LogTemp, Log, TEXT("6           %f / %f / %f"), MonAngle, Min, Max);
				if (MonAngle >= StandardAngle && MonAngle < Min)
				{
					AI->BBComponent->SetValueAsInt("RotateCheck", 1);
					RagdollDog->CurrentDogAnimState = EDogAnimState::SideWalk;
					RagdollDog->CurrentDogCircleState = EDogCircleState::RightCircle;
				}
				else if (MonAngle < StandardAngle || MonAngle > Max)
				{
					AI->BBComponent->SetValueAsInt("RotateCheck", 2);
					RagdollDog->CurrentDogAnimState = EDogAnimState::SideWalk;
					RagdollDog->CurrentDogCircleState = EDogCircleState::LeftCircle;
				}
				else
				{
					RagdollDog->CurrentDogAnimState = EDogAnimState::JumpAttack;

					if (!RagdollDog->bIsAttack)
						RagdollDog->CurrentDogJumpState = EDogJumpState::Nothing;		// SetJumpStart에서 JumpStart로 자동 세팅

					RagdollDog->GetCharacterMovement()->MaxWalkSpeed = 550.0f;
				}
			}
		}
	}
	
}
