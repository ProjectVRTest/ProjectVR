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

		if (Distance > 400.0f)
		{
			RagdollDog->CurrentDogState = EDogState::Chase;
			RagdollDog->CurrentDogAnimState = EDogAnimState::Run;
			RagdollDog->CurrentDogJumpState = EDogJumpState::Nothing;
			RagdollDog->CurrentDogCircleState = EDogCircleState::Nothing;
		}

		if (RagdollDog->Landing || RagdollDog->AttachActor || RagdollDog->bIsAttack)
			return;

		if (RagdollDog && MyCharacter)
		{
			RagdollDog->bInAttackplace = false;

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
				if (RagdollDog->bIsLeftWander || MonAngle >= StandardAngle && MonAngle < Min)
				{
					RightRange(AI, RagdollDog);

					if (MonAngle < StandardAngle && MonAngle > Max)
					{
						RagdollDog->bIsLeftWander = false;
						RagdollDog->bIsRightWander = true;
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
	RagdollDog->bInAttackplace = true;
	RagdollDog->bAttack = false;

	if (!RagdollDog->once)
	{
		MyCharacter->DogArray.Add(RagdollDog);
		RagdollDog->once = true;
	}

	ADog** Dogs = MyCharacter->DogArray.GetData();

	for (int i = 0; i < MyCharacter->DogArray.Num(); i++)
	{
		if (Dogs[i] != RagdollDog)		// 자기랑 아닌거랑 비교
		{
			RagdollDog->bAttack = false;		// 공격 불가
			return;
		}
		else
		{
			RagdollDog->bAttack = true;
			break;
		}
	}

	if (RagdollDog->bAttack)
	{
		UE_LOG(LogClass, Warning, TEXT("Service3 %s"), *RagdollDog->GetName());
		RagdollDog->CurrentDogState = EDogState::Battle;
		RagdollDog->CurrentDogAnimState = EDogAnimState::JumpAttack;

		if (!RagdollDog->bIsAttack)
		{
			RagdollDog->CurrentDogJumpState = EDogJumpState::Nothing;		// SetJumpStart에서 JumpStart로 자동 세팅
		}
		RagdollDog->GetCharacterMovement()->MaxWalkSpeed = 550.0f;
	}
}

void UBTService_CheckCanAttack::LeftRange(ADogAIController* AI, ADog * RagdollDog)
{
	AI->BBComponent->SetValueAsInt("RotateCheck", 2);
	RagdollDog->CurrentDogState = EDogState::Circle;
	RagdollDog->CurrentDogAnimState = EDogAnimState::SideWalk;
	RagdollDog->CurrentDogCircleState = EDogCircleState::LeftCircle;
	RagdollDog->bIsLeftWander = false;
	RagdollDog->bIsRightWander = true;
}

void UBTService_CheckCanAttack::RightRange(ADogAIController * AI, ADog * RagdollDog)
{
	AI->BBComponent->SetValueAsInt("RotateCheck", 1);
	RagdollDog->CurrentDogState = EDogState::Circle;
	RagdollDog->CurrentDogAnimState = EDogAnimState::SideWalk;
	RagdollDog->CurrentDogCircleState = EDogCircleState::RightCircle;
	RagdollDog->bIsLeftWander = true;
	RagdollDog->bIsRightWander = false;
}

void UBTService_CheckCanAttack::UnAttackableRange(AMotionControllerCharacter * MyCharacter, ADog * RagdollDog)
{
	if (!RagdollDog->AttachActor)		// 물고있을 때는 다른 개들이 물지 못하도록 배열삭제를 막음
	{
		RagdollDog->bInAttackplace = false;
		MyCharacter->DogArray.Remove(RagdollDog);
		RagdollDog->once = false;
	}
}

