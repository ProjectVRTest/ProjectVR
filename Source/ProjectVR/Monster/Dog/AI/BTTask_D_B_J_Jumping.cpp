// Fill out your copyright notice in the Description page of Project Settings.

#include "BTTask_D_B_J_Jumping.h"
#include "Monster/Dog/DogAIController.h"
#include "Monster/Dog/Dog.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Components/CapsuleComponent.h"								// 땅과의 거리 판단을 위해서 필요한 컴포넌트
#include "Components/BoxComponent.h"									// 입 콜리전 활성화/비활성화를 위한 컴포넌트
#include "Components/SkeletalMeshComponent.h"						// 개의 래그돌화를 위해서 필요한 컴포넌트
#include "BehaviorTree/BlackboardComponent.h"							// 커스텀 대기시간
#include "MyCharacter/MotionControllerCharacter.h"

void UBTTask_D_B_J_Jumping::InitializeFromAsset(UBehaviorTree & Asset)
{
	Super::InitializeFromAsset(Asset);

	bNotifyTick = true;				// 틱을 실행할 것인지 

	// 죽었는지?
	bIsDeath = false;

	// 공중 상태 초기화
	CurrentFalling = false;
	PreviousFalling = false;
}

EBTNodeResult::Type UBTTask_D_B_J_Jumping::ExecuteTask(UBehaviorTreeComponent & OwnerComp, uint8 * NodeMemory)
{
	// Dog 캐스트 작업
	AI = Cast<ADogAIController>(OwnerComp.GetAIOwner());
	if (AI)
	{
		Dog = Cast<ADog>(AI->GetPawn());
		AActor* Player = Cast<AActor>(AI->BBComponent->GetValueAsObject(TEXT("Player")));
		MyCharacter = Cast<AMotionControllerCharacter>(Player);
	}
	return EBTNodeResult::InProgress;			// Tick
}

void UBTTask_D_B_J_Jumping::TickTask(UBehaviorTreeComponent & OwnerComp, uint8 * NodeMemory, float DeltaSeconds)
{
	Super::TickTask(OwnerComp, NodeMemory, DeltaSeconds);

	if (Dog)
	{
		// 체력 검사
		if (AI->BBComponent->GetValueAsFloat("HP") <= 0.0f)		// 커스텀 대기시간(죽음) <= 0)
		{
			bIsDeath = true;		// 죽음
			Dog->GetMesh()->SetAllBodiesBelowSimulatePhysics("Bip002-Neck", true, true);			// Neck이하는 모조리 피직스를 줌
		}

		FFindFloorResult FloorDistance;
		CurrentFalling = Dog->CurrentFalling;

		// 공중에 있는 상태
		if (CurrentFalling && PreviousFalling)
		{
			Dog->CurrentDogJumpState = EDogJumpState::JumpRoof;

			// 개가 플레이어를 물었는지를 판단하고 Biting 상태로 바꿔줌
			if (AI->BBComponent->GetValueAsBool("bIsBiting"))
			{
				Dog->DogAttackCollision->bGenerateOverlapEvents = false;			// 입 콜리전 비활성화
				Dog->CurrentDogState = EDogState::Battle;
				Dog->CurrentDogBattleState = EDogBattleState::Biting;
				FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);				// 틱 종료
			}
		}

		// 땅에 착지한 상태
		if (!CurrentFalling && PreviousFalling)
		{
			Dog->GetCharacterMovement()->ComputeFloorDist(Dog->GetCapsuleComponent()->GetComponentLocation(), 10000.0f, 10000.0f, FloorDistance, 34.0f);

			// 거리차가 만족하면 실행
			if (FloorDistance.FloorDist < 2.3f)
			{
				if (bIsDeath)
				{
					AI->BBComponent->SetValueAsFloat("CustomWaitTime", 0.0f);		// 커스텀 대기시간(죽음)
					FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);				// 틱 종료
				}
				else
				{
					if (MyCharacter->DogArray.Contains(Dog))			// 배열에 개가 있으면
					{
						MyCharacter->DogArray.Remove(Dog);		// 제거
					}
					AI->BBComponent->SetValueAsFloat("CustomWaitTime", 1.0f);		// 커스텀 대기시간(착지)
					Dog->DogAttackCollision->bGenerateOverlapEvents = false;
					Dog->CurrentDogJumpState = EDogJumpState::JumpEnd;				// 착지
					FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);				// 틱 종료
				}
			}
		}

		PreviousFalling = CurrentFalling;
	}
}
