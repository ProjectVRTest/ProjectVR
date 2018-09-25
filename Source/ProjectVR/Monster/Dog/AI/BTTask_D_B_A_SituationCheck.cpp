// Fill out your copyright notice in the Description page of Project Settings.

#include "BTTask_D_B_A_SituationCheck.h"

#include "Monster/Dog/DogAIController.h"
#include "Monster/Dog/Dog.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Components/CapsuleComponent.h"								// 땅과의 거리 판단을 위해서 필요한 컴포넌트
#include "Components/BoxComponent.h"									// 입 콜리전 활성화/비활성화를 위한 컴포넌트
#include "Components/SkeletalMeshComponent.h"						// 개의 래그돌화를 위해서 필요한 컴포넌트
#include "BehaviorTree/BlackboardComponent.h"							// 커스텀 대기시간
#include "Kismet/KismetMathLibrary.h"
#include "Animation/AnimInstance.h"

void UBTTask_D_B_A_SituationCheck::InitializeFromAsset(UBehaviorTree & Asset)
{
	Super::InitializeFromAsset(Asset);

	bNotifyTick = true;				// 틱을 실행할 것인지 

	// 죽었는지?
	bIsDeath = false;

	// 공중 상태 초기화
	CurrentFalling = false;
	PreviousFalling = false;
}

EBTNodeResult::Type UBTTask_D_B_A_SituationCheck::ExecuteTask(UBehaviorTreeComponent & OwnerComp, uint8 * NodeMemory)
{
	// Dog 캐스트 작업
	AI = Cast<ADogAIController>(OwnerComp.GetAIOwner());
	if (AI)
	{
		Dog = Cast<ADog>(AI->GetPawn());
		Dog->AttachActor = NULL;					// 공중 상태이므로 현재 물고있는 액터 존재안함
	}
	return EBTNodeResult::InProgress;			// Tick
}

void UBTTask_D_B_A_SituationCheck::TickTask(UBehaviorTreeComponent & OwnerComp, uint8 * NodeMemory, float DeltaSeconds)
{
	Super::TickTask(OwnerComp, NodeMemory, DeltaSeconds);

	if (Dog)
	{
		FFindFloorResult FloorDistance;

		Dog->GetCharacterMovement()->ComputeFloorDist(Dog->GetCapsuleComponent()->GetComponentLocation(), 10000.0f, 10000.0f, FloorDistance, 34.0f);

		// 거리차가 만족하면 실행
		if (FloorDistance.FloorDist < 2.5f)
		{
			if (AI->BBComponent->GetValueAsFloat("HP") <= 0)		// 커스텀 대기시간(죽음) <= 0)
				bIsDeath = true;		// 죽음
			else
				bIsDeath = false;		// 생존

			AActor* Player = Cast<AActor>(AI->BBComponent->GetValueAsObject("Player"));
			FRotator LookAt = UKismetMathLibrary::FindLookAtRotation(Dog->GetActorLocation(), Player->GetActorLocation());

			if (bIsDeath)
			{
				Dog->GetCapsuleComponent()->SetRelativeRotation(FRotator(0.0f, LookAt.Yaw, 0.0f));
				Dog->GetMesh()->SetAllBodiesBelowSimulatePhysics("Bip002-Head", true, true);		// 동작가능하게 함
				Dog->GetCapsuleComponent()->SetSimulatePhysics(true);				// 동작 가능하게 함
				AI->BBComponent->SetValueAsFloat("CustomWaitTime", 2.5f);		// 커스텀 대기시간(죽음)
				FinishLatentTask(OwnerComp, EBTNodeResult::Failed);				// 틱 종료
			}
			else
			{
				Dog->GetCapsuleComponent()->SetRelativeRotation(FRotator(0.0f, LookAt.Yaw, 0.0f));
				Dog->GetMesh()->SetAllBodiesBelowSimulatePhysics("Bip002-Neck", false, true);		// 동작가능하게 함
				Dog->GetCapsuleComponent()->SetSimulatePhysics(false);				// 동작 가능하게 함

				AI->BBComponent->SetValueAsFloat("CustomWaitTime", 0.3f);		// 커스텀 대기시간(일어나는시간)
				Dog->CurrentDogAirState = EDogAirState::GetUp;
				FinishLatentTask(OwnerComp, EBTNodeResult::Failed);				// 틱 종료
			}
		}

	}
}