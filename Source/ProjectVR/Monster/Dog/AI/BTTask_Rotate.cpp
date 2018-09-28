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

				// 개가 플레이어를 바라는보는 각도를 구함
				FRotator LookAt = UKismetMathLibrary::FindLookAtRotation(RagdollDog->GetActorLocation(), MyCharacter->Camera->GetComponentLocation());

				if (RagdollDog)
				{
					// 플레이어가 볼수없는 각도가 존재할수 있으니까 연속적으로 회전하는것이 무의미하므로 바로 회전시킴
					LookAt.Pitch = 0;
					RagdollDog->SetActorRotation(LookAt);	// 바로 회전

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

