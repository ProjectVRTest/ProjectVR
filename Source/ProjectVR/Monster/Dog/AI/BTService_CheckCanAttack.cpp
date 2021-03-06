﻿// Fill out your copyright notice in the Description page of Project Settings.

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

	Range = 30.0f;
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

		float Distance = FVector::Distance(RagdollDog->GetActorLocation(), MyCharacter->Camera->GetComponentLocation());

		// 개와의 거리가 멀어질 때
		if (Distance > 400.0f)
		{
			RagdollDog->CurrentDogState = EDogState::Chase;
			RagdollDog->CurrentDogAnimState = EDogAnimState::Run;
			RagdollDog->CurrentDogBattleState = EDogBattleState::Nothing;
			RagdollDog->CurrentDogJumpState = EDogJumpState::Nothing;
			RagdollDog->CurrentDogCircleState = EDogCircleState::Nothing;
			UnAttackableRange(MyCharacter, RagdollDog);
			return;
		}

		// 개가 죽었을 때
		if (RagdollDog->CurrentHP <= 0.0f)
		{
			RagdollDog->CurrentDogState = EDogState::Death;
			RagdollDog->CurrentDogAnimState = EDogAnimState::Nothing;
			RagdollDog->CurrentDogBattleState = EDogBattleState::Nothing;
			RagdollDog->CurrentDogJumpState = EDogJumpState::Nothing;
			RagdollDog->CurrentDogCircleState = EDogCircleState::Nothing;
			UnAttackableRange(MyCharacter, RagdollDog);
			return;
		}

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

			// 주위를 도는 경우의 수는 6가지, 개의 보는 각도랑 플레이어와 보는 각도 반전 -------------------------------------------------------------------- 주석 대기
			if (StandardAngle <= Range && StandardAngle >= 0.0f)
			{
				if (RagdollDog->bIsLeftWander || MonAngle >= StandardAngle && MonAngle < Min)
				{
					RightRange(AI, RagdollDog);

					if (MonAngle < StandardAngle || MonAngle > Max)
					{
						RagdollDog->bIsLeftWander = false;
						RagdollDog->bIsRightWander = true;
						return;
					}

					if ((MonAngle < StandardAngle || MonAngle > Max) || (MonAngle >= StandardAngle && MonAngle < Min))
						UnAttackableRange(MyCharacter, RagdollDog);
					else
						AttackableRange(MyCharacter, RagdollDog);
				}
				else if (RagdollDog->bIsRightWander || MonAngle < StandardAngle || MonAngle > Max)
				{
					LeftRange(AI, RagdollDog);

					if (MonAngle >= StandardAngle && MonAngle < Min)
					{
						RagdollDog->bIsLeftWander = true;
						RagdollDog->bIsRightWander = false;
						return;
					}

					if ((MonAngle < StandardAngle || MonAngle > Max) || (MonAngle >= StandardAngle && MonAngle < Min))
						UnAttackableRange(MyCharacter, RagdollDog);
					else
						AttackableRange(MyCharacter, RagdollDog);
				}
				else
					AttackableRange(MyCharacter, RagdollDog);
			}
			else if (StandardAngle >= 360.0f - Range && StandardAngle >= 0.0f)
			{
				if (RagdollDog->bIsLeftWander || MonAngle >= StandardAngle || MonAngle < Min)
				{
					RightRange(AI, RagdollDog);

					if (MonAngle < StandardAngle && MonAngle > Max)
					{
						RagdollDog->bIsLeftWander = false;
						RagdollDog->bIsRightWander = true;
						return;
					}

					if ((MonAngle >= StandardAngle || MonAngle < Min) || (MonAngle < StandardAngle && MonAngle > Max))
						UnAttackableRange(MyCharacter, RagdollDog);
					else
						AttackableRange(MyCharacter, RagdollDog);
				}
				else if (RagdollDog->bIsRightWander || MonAngle < StandardAngle && MonAngle > Max)
				{
					LeftRange(AI, RagdollDog);

					if (MonAngle >= StandardAngle || MonAngle < Min)
					{
						RagdollDog->bIsLeftWander = true;
						RagdollDog->bIsRightWander = false;
						return;
					}

					if ((MonAngle >= StandardAngle || MonAngle < Min) || (MonAngle < StandardAngle && MonAngle > Max))
						UnAttackableRange(MyCharacter, RagdollDog);
					else
						AttackableRange(MyCharacter, RagdollDog);
				}
				else
					AttackableRange(MyCharacter, RagdollDog);
			}
			else if (StandardAngle <= 180.0f + Range && StandardAngle >= 180.0f)	// 3
			{
				if (RagdollDog->bIsLeftWander || (MonAngle >= StandardAngle && MonAngle < Min))
				{
					RightRange(AI, RagdollDog);

					if (MonAngle < StandardAngle && MonAngle > Max)
					{
						RagdollDog->bIsLeftWander = false;
						RagdollDog->bIsRightWander = true;
						return;
					}

					if ((MonAngle >= StandardAngle && MonAngle < Min) || (MonAngle < StandardAngle && MonAngle > Max))		// 공격 범위를 벗어남
						UnAttackableRange(MyCharacter, RagdollDog);	
					else		// 공격범위에 있음
						AttackableRange(MyCharacter, RagdollDog);
				}
				else if (RagdollDog->bIsRightWander || (MonAngle < StandardAngle && MonAngle > Max))
				{
					LeftRange(AI, RagdollDog);

					if (MonAngle >= StandardAngle && MonAngle < Min)
					{
						RagdollDog->bIsLeftWander = true;
						RagdollDog->bIsRightWander = false;
						return;
					}

					if ((MonAngle >= StandardAngle && MonAngle < Min) || (MonAngle < StandardAngle && MonAngle > Max))
						UnAttackableRange(MyCharacter, RagdollDog);
					else
						AttackableRange(MyCharacter, RagdollDog);
				}
				else
					AttackableRange(MyCharacter, RagdollDog);
			}
			else if (StandardAngle >= 180.0f - Range && StandardAngle <= 180.0f)	// 4
			{
				if (RagdollDog->bIsLeftWander || MonAngle >= StandardAngle && MonAngle < Min)
				{
					RightRange(AI, RagdollDog);

					if (MonAngle < StandardAngle && MonAngle > Max)
					{
						RagdollDog->bIsLeftWander = false;
						RagdollDog->bIsRightWander = true;
						return;
					}

					if ((MonAngle >= StandardAngle && MonAngle < Min) || (MonAngle < StandardAngle && MonAngle > Max))
						UnAttackableRange(MyCharacter, RagdollDog);
					else
						AttackableRange(MyCharacter, RagdollDog);
				}
				else if (RagdollDog->bIsRightWander || MonAngle < StandardAngle && MonAngle > Max)
				{
					LeftRange(AI, RagdollDog);

					if (MonAngle >= StandardAngle && MonAngle < Min)
					{
						RagdollDog->bIsLeftWander = true;
						RagdollDog->bIsRightWander = false;
						return;
					}

					if ((MonAngle >= StandardAngle && MonAngle < Min) || (MonAngle < StandardAngle && MonAngle > Max))
						UnAttackableRange(MyCharacter, RagdollDog);
					else
						AttackableRange(MyCharacter, RagdollDog);
				}
				else
					AttackableRange(MyCharacter, RagdollDog);
			}
			else if (StandardAngle < 360.0f - Range && StandardAngle > 180.0f + Range)	// 5
			{
				if (RagdollDog->bIsLeftWander || MonAngle >= StandardAngle || MonAngle < Min)
				{
					RightRange(AI, RagdollDog);

					if (MonAngle < StandardAngle && MonAngle > Max)
					{
						RagdollDog->bIsLeftWander = false;
						RagdollDog->bIsRightWander = true;
						return;
					}

					if ((MonAngle >= StandardAngle || MonAngle < Min) || (MonAngle < StandardAngle && MonAngle > Max))
						UnAttackableRange(MyCharacter, RagdollDog);
					else
						AttackableRange(MyCharacter, RagdollDog);
				}
				else if (RagdollDog->bIsRightWander || MonAngle < StandardAngle && MonAngle > Max)
				{
					LeftRange(AI, RagdollDog);

					if (MonAngle >= StandardAngle || MonAngle < Min)
					{
						RagdollDog->bIsLeftWander = true;
						RagdollDog->bIsRightWander = false;
						return;
					}

					if ((MonAngle >= StandardAngle || MonAngle < Min) || (MonAngle < StandardAngle && MonAngle > Max))
						UnAttackableRange(MyCharacter, RagdollDog);
					else
						AttackableRange(MyCharacter, RagdollDog);
				}
				else
					AttackableRange(MyCharacter, RagdollDog);
			}
			else if (StandardAngle > 0.0f + Range && StandardAngle < 180.0f - Range)	// 6
			{
				if (RagdollDog->bIsLeftWander || MonAngle >= StandardAngle && MonAngle < Min)
				{
					RightRange(AI, RagdollDog);

					if (MonAngle < StandardAngle || MonAngle > Max)
					{
						RagdollDog->bIsLeftWander = false;
						RagdollDog->bIsRightWander = true;
						return;
					}

					if ((MonAngle >= StandardAngle && MonAngle < Min) || (MonAngle < StandardAngle || MonAngle > Max))
						UnAttackableRange(MyCharacter, RagdollDog);
					else
						AttackableRange(MyCharacter, RagdollDog);
				}
				else if (RagdollDog->bIsRightWander || MonAngle < StandardAngle || MonAngle > Max)
				{
					LeftRange(AI, RagdollDog);

					if (MonAngle >= StandardAngle && MonAngle < Min)
					{
						RagdollDog->bIsLeftWander = true;
						RagdollDog->bIsRightWander = false;
						return;
					}

					if ((MonAngle >= StandardAngle && MonAngle < Min) || (MonAngle < StandardAngle || MonAngle > Max))
						UnAttackableRange(MyCharacter, RagdollDog);
					else
						AttackableRange(MyCharacter, RagdollDog);
				}
				else
					AttackableRange(MyCharacter, RagdollDog);
			}
		}
	}

}

void UBTService_CheckCanAttack::SetRandomCircle(ADog * RagdollDog)
{
	int random = FMath::RandRange(0, 1);

	if (random == 0)
	{
		RagdollDog->bIsLeftWander = false;
		RagdollDog->bIsRightWander = true;
	}
	else
	{
		RagdollDog->bIsLeftWander = true;
		RagdollDog->bIsRightWander = false;
	}
}

void UBTService_CheckCanAttack::AttackableRange(AMotionControllerCharacter* MyCharacter, ADog * RagdollDog)
{
	if (!MyCharacter->DogArray.Contains(RagdollDog))			// 배열에 개가 없으면
		MyCharacter->DogArray.Add(RagdollDog);					// 추가;
	
	ADog** Dogs = MyCharacter->DogArray.GetData();

	if (Dogs[0] != RagdollDog)		// 자기랑 아닌거랑 비교
	{
		RagdollDog->bAttack = false;		// 공격 불가

		if (!RagdollDog->AttackWaite)		// 여러마리가 있을 때 / 공격을 할 수 없다면 랜덤한 방향으로 배회하도록 한다
		{
			SetRandomCircle(RagdollDog);
			RagdollDog->AttackWaite = true;
		}
		return;
	}
	else
		RagdollDog->bAttack = true;

	if (RagdollDog->bAttack)
	{
		RagdollDog->CurrentDogState = EDogState::Battle;
		RagdollDog->CurrentDogBattleState = EDogBattleState::JumpAttack;
		RagdollDog->CurrentDogJumpState = EDogJumpState::JumpStart;
		RagdollDog->GetCharacterMovement()->MaxWalkSpeed = 550.0f;
	}
}

void UBTService_CheckCanAttack::LeftRange(ADogAIController* AI, ADog * RagdollDog)
{
	AI->BBComponent->SetValueAsInt("RotateCheck", 2);

	RagdollDog->CurrentDogState = EDogState::Battle;
	RagdollDog->CurrentDogBattleState = EDogBattleState::Circle;
	RagdollDog->CurrentDogJumpState = EDogJumpState::Nothing;
	RagdollDog->CurrentDogCircleState = EDogCircleState::LeftCircle;

	RagdollDog->bIsLeftWander = false;
	RagdollDog->bIsRightWander = true;
}

void UBTService_CheckCanAttack::RightRange(ADogAIController * AI, ADog * RagdollDog)
{
	AI->BBComponent->SetValueAsInt("RotateCheck", 1);

	RagdollDog->CurrentDogState = EDogState::Battle;
	RagdollDog->CurrentDogBattleState = EDogBattleState::Circle;
	RagdollDog->CurrentDogJumpState = EDogJumpState::Nothing;
	RagdollDog->CurrentDogCircleState = EDogCircleState::RightCircle;

	RagdollDog->bIsLeftWander = true;
	RagdollDog->bIsRightWander = false;
}

void UBTService_CheckCanAttack::UnAttackableRange(AMotionControllerCharacter * MyCharacter, ADog * RagdollDog)
{
	if (!RagdollDog->AttachActor)		// 물고있을 때는 다른 개들이 물지 못하도록 배열삭제를 막음
	{
		if (MyCharacter->DogArray.Contains(RagdollDog))			// 배열에 개가 있으면
		{
			MyCharacter->DogArray.Remove(RagdollDog);			// 제거
		}

	}
}

// UE_LOG(LogTemp, Log, TEXT("mmmmmmmmmmmmmmmmmmmm"));