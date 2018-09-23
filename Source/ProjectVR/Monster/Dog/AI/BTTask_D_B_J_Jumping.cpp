// Fill out your copyright notice in the Description page of Project Settings.

#include "BTTask_D_B_J_Jumping.h"
#include "Monster/Dog/DogAIController.h"
#include "Monster/Dog/Dog.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Components/CapsuleComponent.h"								// ������ �Ÿ� �Ǵ��� ���ؼ� �ʿ��� ������Ʈ
#include "Components/BoxComponent.h"									// �� �ݸ��� Ȱ��ȭ/��Ȱ��ȭ�� ���� ������Ʈ
#include "Components/SkeletalMeshComponent.h"						// ���� ���׵�ȭ�� ���ؼ� �ʿ��� ������Ʈ
#include "BehaviorTree/BlackboardComponent.h"							// Ŀ���� ���ð�

void UBTTask_D_B_J_Jumping::InitializeFromAsset(UBehaviorTree & Asset)
{
	Super::InitializeFromAsset(Asset);

	bNotifyTick = true;				// ƽ�� ������ ������ 

	// �׾�����?
	bIsDeath = false;

	// ���� ���� �ʱ�ȭ
	CurrentFalling = false;
	PreviousFalling = false;
}

EBTNodeResult::Type UBTTask_D_B_J_Jumping::ExecuteTask(UBehaviorTreeComponent & OwnerComp, uint8 * NodeMemory)
{
	// Dog ĳ��Ʈ �۾�
	AI = Cast<ADogAIController>(OwnerComp.GetAIOwner());
	if (AI)
	{
		Dog = Cast<ADog>(AI->GetPawn());
	}
	return EBTNodeResult::InProgress;			// Tick
}

void UBTTask_D_B_J_Jumping::TickTask(UBehaviorTreeComponent & OwnerComp, uint8 * NodeMemory, float DeltaSeconds)
{
	Super::TickTask(OwnerComp, NodeMemory, DeltaSeconds);

	if (Dog)
	{
		// ü�� �˻�
		if (AI->BBComponent->GetValueAsInt("HP") <= 0)		// Ŀ���� ���ð�(����) <= 0)
		{
			bIsDeath = true;		// ����
			Dog->GetMesh()->SetAllBodiesBelowSimulatePhysics("Bip002-Neck", true, true);			// Neck���ϴ� ������ �������� ��
		}

		FFindFloorResult FloorDistance;
		CurrentFalling = Dog->CurrentFalling;

		// ���߿� �ִ� ����
		if (CurrentFalling && PreviousFalling)
		{
			Dog->CurrentDogJumpState = EDogJumpState::JumpRoof;

			// ���� �÷��̾ ���������� �Ǵ��ϰ� Biting ���·� �ٲ���
			// �÷��̾��� �Ӹ��� Ʈ���ŵƴٴ� �÷��׸� ����
			// �÷��װ� True�� �� 
			// {
			// �� �ݸ��� ��Ȱ��ȭ
			// Dog->CurrentDogBattleState = EDogBattleState::Biting;
			// FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);				// ƽ ����
			// }
		}

		// ���� ������ ����
		if (!CurrentFalling && PreviousFalling)
		{
			Dog->GetCharacterMovement()->ComputeFloorDist(Dog->GetCapsuleComponent()->GetComponentLocation(), 10000.0f, 10000.0f, FloorDistance, 34.0f);

			// �Ÿ����� �����ϸ� ����
			if (FloorDistance.FloorDist < 230.0f)
			{
				if (bIsDeath)
				{
					AI->BBComponent->SetValueAsFloat("CustomWaitTime", 0.0f);		// Ŀ���� ���ð�(����)
					FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);				// ƽ ����
				}
				else
				{
					AI->BBComponent->SetValueAsFloat("CustomWaitTime", 0.3f);		// Ŀ���� ���ð�(����)
					Dog->DogAttackCollision->bGenerateOverlapEvents = false;
					Dog->CurrentDogJumpState = EDogJumpState::JumpEnd;				// ����
					Dog->CurrentDogBattleState = EDogBattleState::Circle;					// �ٽ� ��ȸ����
					FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);				// ƽ ����
				}
			}
		}

		PreviousFalling = CurrentFalling;
	}
}
