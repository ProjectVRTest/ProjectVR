// Fill out your copyright notice in the Description page of Project Settings.

#include "BTTask_D_B_B_Biting.h"

#include "Monster/Dog/DogAIController.h"
#include "Monster/Dog/Dog.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Components/CapsuleComponent.h"								// ������ �Ÿ� �Ǵ��� ���ؼ� �ʿ��� ������Ʈ
#include "Components/BoxComponent.h"									// �� �ݸ��� Ȱ��ȭ/��Ȱ��ȭ�� ���� ������Ʈ
#include "Components/SkeletalMeshComponent.h"						// ���� ���׵�ȭ�� ���ؼ� �ʿ��� ������Ʈ
#include "BehaviorTree/BlackboardComponent.h"							// Ŀ���� ���ð�
#include "HandMotionController/RightHandMotionController.h"
#include "MyCharacter/MotionControllerCharacter.h"
#include "Components/CapsuleComponent.h"
#include "Camera/CameraComponent.h"

void UBTTask_D_B_B_Biting::InitializeFromAsset(UBehaviorTree & Asset)
{
	Super::InitializeFromAsset(Asset);

	bNotifyTick = true;				// ƽ�� ������ ������ 

}

EBTNodeResult::Type UBTTask_D_B_B_Biting::ExecuteTask(UBehaviorTreeComponent & OwnerComp, uint8 * NodeMemory)
{
	// Dog ĳ��Ʈ �۾�
	AI = Cast<ADogAIController>(OwnerComp.GetAIOwner());
	if (AI)
	{
		Dog = Cast<ADog>(AI->GetPawn());
	}
	return EBTNodeResult::InProgress;			// Tick
}

void UBTTask_D_B_B_Biting::TickTask(UBehaviorTreeComponent & OwnerComp, uint8 * NodeMemory, float DeltaSeconds)
{
	Super::TickTask(OwnerComp, NodeMemory, DeltaSeconds);

	if (Dog->AttachActor)
	{
		// ���� ���� �̻��� ���� ü���� 0������ ��, ����� ����
		if (AI->BBComponent->GetValueAsInt("DetachStack") >= MaxStack || AI->BBComponent->GetValueAsFloat("HP") <= 0)
		{
			if (AI->BBComponent->GetValueAsObject("AttachActor"))
			{
				ARightHandMotionController* RightController = Cast<ARightHandMotionController>(AI->BBComponent->GetValueAsObject("AttachActor"));
				if (RightController)
				{
					AMotionControllerCharacter* Player = Cast<AMotionControllerCharacter>(RightController->HandOwner);
					if (Player)
					{
						// ���󰡴� ����
						FVector Direction = Player->Camera->GetUpVector() + Player->Camera->GetForwardVector();

						// ���󰡴� ���� ����
						Dog->GetCapsuleComponent()->SetPhysicsLinearVelocity(Direction* 500.0f);
						Dog->GetCapsuleComponent()->SetPhysicsAngularVelocity(Direction* 500.0f);
						Dog->GetCapsuleComponent()->SetSimulatePhysics(true);
						Dog->GetCapsuleComponent()->AddForce(Direction * 500.0f);

						Dog->GetMesh()->SetAllBodiesBelowSimulatePhysics("Bip002-Neck", true, true);		// �� ����
						Dog->CurrentDogBattleState = EDogBattleState::Air;				// ���� ����
						FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);				// ƽ ����
					}
				}
			}


		}
	}
}
