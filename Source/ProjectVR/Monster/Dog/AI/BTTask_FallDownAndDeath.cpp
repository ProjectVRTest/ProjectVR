// Fill out your copyright notice in the Description page of Project Settings.

#include "BTTask_FallDownAndDeath.h"
#include "Monster/Dog/Dog.h"
#include "Monster/Dog/DogAIController.h"
#include "MyCharacter/MotionControllerCharacter.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Components/SkeletalMeshComponent.h"						// ���� ���׵�ȭ�� ���ؼ� �ʿ��� ������Ʈ
#include "Components/CapsuleComponent.h"
EBTNodeResult::Type UBTTask_FallDownAndDeath::ExecuteTask(UBehaviorTreeComponent & OwnerComp, uint8 * NodeMemory)
{
	ADogAIController* AI = Cast<ADogAIController>(OwnerComp.GetAIOwner());
	
	AActor* Player = Cast<AActor>(AI->BBComponent->GetValueAsObject(TEXT("Player")));
	AMotionControllerCharacter* MyCharacter = Cast<AMotionControllerCharacter>(Player);

	if (AI)
	{
		ADog* Dog = Cast<ADog>(AI->GetPawn());
		if (Dog)
		{
			if (MyCharacter)
			{
				if (MyCharacter->DogArray.Contains(Dog))
					MyCharacter->DogArray.Remove(Dog);
			}

			Dog->GetMesh()->SetAllBodiesBelowSimulatePhysics("Bip002-Head", true, true);		// ���۰����ϰ� ��
			Dog->GetMesh()->SetSimulatePhysics(true);
			Dog->GetCapsuleComponent()->SetSimulatePhysics(false);				// ���� �����ϰ� ��
			AI->BBComponent->SetValueAsFloat("CustomWaitTime", 2.5f);		// Ŀ���� ���ð�(����)

			return EBTNodeResult::Succeeded;
		}
	}
	return EBTNodeResult::Failed;
}
