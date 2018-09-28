// Fill out your copyright notice in the Description page of Project Settings.

#include "BTTask_Rotate.h"

#include "Monster/Dog/Dog.h"
#include "Monster/Dog/DogAIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "MyCharacter/MotionControllerCharacter.h"
#include "Kismet/KismetMathLibrary.h"
#include "Camera/CameraComponent.h"

void UBTTask_Rotate::InitializeFromAsset(UBehaviorTree & Asset)
{
	Super::InitializeFromAsset(Asset);
}

EBTNodeResult::Type UBTTask_Rotate::ExecuteTask(UBehaviorTreeComponent & OwnerComp, uint8 * NodeMemory)
{
	ADogAIController* AI = Cast<ADogAIController>(OwnerComp.GetAIOwner());

	if (AI)
	{
		AActor* Player = Cast<AActor>(AI->BBComponent->GetValueAsObject(TEXT("Player")));

		if (Player)
		{
			AMotionControllerCharacter* MyCharacter = Cast<AMotionControllerCharacter>(Player);

			if (MyCharacter)
			{
				ADog* RagdollDog = Cast<ADog>(AI->GetPawn());

				// ���� �÷��̾ �ٶ�º��� ������ ����
				FRotator LookAt = UKismetMathLibrary::FindLookAtRotation(RagdollDog->GetActorLocation(), MyCharacter->Camera->GetComponentLocation());

				if (RagdollDog)
				{
					// �÷��̾ �������� ������ �����Ҽ� �����ϱ� ���������� ȸ���ϴ°��� ���ǹ��ϹǷ� �ٷ� ȸ����Ŵ
					LookAt.Pitch = 0;
					RagdollDog->SetActorRotation(LookAt);	// �ٷ� ȸ��

					return EBTNodeResult::Succeeded;
				}
				else
				{
					return EBTNodeResult::Failed;
				}
			}
			else
			{
				return EBTNodeResult::Failed;
			}
		}
		else
		{
			return EBTNodeResult::Failed;
		}
	}
	else
	{
		return EBTNodeResult::Failed;
	}
}

