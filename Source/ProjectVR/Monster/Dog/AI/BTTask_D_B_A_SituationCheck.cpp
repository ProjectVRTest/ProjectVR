// Fill out your copyright notice in the Description page of Project Settings.

#include "BTTask_D_B_A_SituationCheck.h"

#include "Monster/Dog/DogAIController.h"
#include "Monster/Dog/Dog.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Components/CapsuleComponent.h"								// ������ �Ÿ� �Ǵ��� ���ؼ� �ʿ��� ������Ʈ
#include "Components/BoxComponent.h"									// �� �ݸ��� Ȱ��ȭ/��Ȱ��ȭ�� ���� ������Ʈ
#include "Components/SkeletalMeshComponent.h"						// ���� ���׵�ȭ�� ���ؼ� �ʿ��� ������Ʈ
#include "BehaviorTree/BlackboardComponent.h"							// Ŀ���� ���ð�

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
		CurrentFalling = Dog->CurrentFalling;

		// ���� ������ ����
		if (!CurrentFalling && PreviousFalling)
		{
			Dog->GetCharacterMovement()->ComputeFloorDist(Dog->GetCapsuleComponent()->GetComponentLocation(), 10000.0f, 10000.0f, FloorDistance, 34.0f);

			// �Ÿ����� �����ϸ� ����
			if (FloorDistance.FloorDist < 230.0f)
			{
				if (AI->BBComponent->GetValueAsInt("HP") <= 0)		// Ŀ���� ���ð�(����) <= 0)
					bIsDeath = true;		// ����
				else
					bIsDeath = false;		// ����

				if (bIsDeath)
				{
					AI->BBComponent->SetValueAsFloat("CustomWaitTime", 0.3f);		// Ŀ���� ���ð�(����)
					FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);				// ƽ ����
				}
				else
				{
					Dog->GetMesh()->SetAllBodiesBelowSimulatePhysics("Bip002-Neck", false, true);		// ���۰����ϰ� ��
					Dog->GetCapsuleComponent()->SetSimulatePhysics(false);				// ���� �����ϰ� ��

					AI->BBComponent->SetValueAsFloat("CustomWaitTime", 0.3f);		// Ŀ���� ���ð�(�Ͼ�½ð�)
					Dog->CurrentDogAirState = EDogAirState::GetUp;
					FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);				// ƽ ����
				}
			}
		}

		PreviousFalling = CurrentFalling;
	}
}