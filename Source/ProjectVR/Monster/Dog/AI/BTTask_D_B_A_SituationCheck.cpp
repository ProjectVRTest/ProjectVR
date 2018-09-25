// Fill out your copyright notice in the Description page of Project Settings.

#include "BTTask_D_B_A_SituationCheck.h"

#include "Monster/Dog/DogAIController.h"
#include "Monster/Dog/Dog.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Components/CapsuleComponent.h"								// ������ �Ÿ� �Ǵ��� ���ؼ� �ʿ��� ������Ʈ
#include "Components/BoxComponent.h"									// �� �ݸ��� Ȱ��ȭ/��Ȱ��ȭ�� ���� ������Ʈ
#include "Components/SkeletalMeshComponent.h"						// ���� ���׵�ȭ�� ���ؼ� �ʿ��� ������Ʈ
#include "BehaviorTree/BlackboardComponent.h"							// Ŀ���� ���ð�
#include "Kismet/KismetMathLibrary.h"
#include "Animation/AnimInstance.h"

void UBTTask_D_B_A_SituationCheck::InitializeFromAsset(UBehaviorTree & Asset)
{
	Super::InitializeFromAsset(Asset);

	bNotifyTick = true;				// ƽ�� ������ ������ 

	// �׾�����?
	bIsDeath = false;

	// ���� ���� �ʱ�ȭ
	CurrentFalling = false;
	PreviousFalling = false;
}

EBTNodeResult::Type UBTTask_D_B_A_SituationCheck::ExecuteTask(UBehaviorTreeComponent & OwnerComp, uint8 * NodeMemory)
{
	// Dog ĳ��Ʈ �۾�
	AI = Cast<ADogAIController>(OwnerComp.GetAIOwner());
	if (AI)
	{
		Dog = Cast<ADog>(AI->GetPawn());
		Dog->AttachActor = NULL;					// ���� �����̹Ƿ� ���� �����ִ� ���� �������
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

		// �Ÿ����� �����ϸ� ����
		if (FloorDistance.FloorDist < 2.5f)
		{
			if (AI->BBComponent->GetValueAsFloat("HP") <= 0)		// Ŀ���� ���ð�(����) <= 0)
				bIsDeath = true;		// ����
			else
				bIsDeath = false;		// ����

			AActor* Player = Cast<AActor>(AI->BBComponent->GetValueAsObject("Player"));
			FRotator LookAt = UKismetMathLibrary::FindLookAtRotation(Dog->GetActorLocation(), Player->GetActorLocation());

			if (bIsDeath)
			{
				Dog->GetCapsuleComponent()->SetRelativeRotation(FRotator(0.0f, LookAt.Yaw, 0.0f));
				Dog->GetMesh()->SetAllBodiesBelowSimulatePhysics("Bip002-Head", true, true);		// ���۰����ϰ� ��
				Dog->GetCapsuleComponent()->SetSimulatePhysics(true);				// ���� �����ϰ� ��
				AI->BBComponent->SetValueAsFloat("CustomWaitTime", 2.5f);		// Ŀ���� ���ð�(����)
				FinishLatentTask(OwnerComp, EBTNodeResult::Failed);				// ƽ ����
			}
			else
			{
				Dog->GetCapsuleComponent()->SetRelativeRotation(FRotator(0.0f, LookAt.Yaw, 0.0f));
				Dog->GetMesh()->SetAllBodiesBelowSimulatePhysics("Bip002-Neck", false, true);		// ���۰����ϰ� ��
				Dog->GetCapsuleComponent()->SetSimulatePhysics(false);				// ���� �����ϰ� ��

				AI->BBComponent->SetValueAsFloat("CustomWaitTime", 0.3f);		// Ŀ���� ���ð�(�Ͼ�½ð�)
				Dog->CurrentDogAirState = EDogAirState::GetUp;
				FinishLatentTask(OwnerComp, EBTNodeResult::Failed);				// ƽ ����
			}
		}

	}
}