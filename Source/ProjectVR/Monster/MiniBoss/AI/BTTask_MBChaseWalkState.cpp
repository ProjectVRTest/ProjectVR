// Fill out your copyright notice in the Description page of Project Settings.

#include "BTTask_MBChaseWalkState.h"
#include "Headers/MiniBossAIHeader.h"

#include "AI/Navigation/NavigationSystem.h"
#include "MyTargetPoint.h"
#include "HeadMountedDisplayFunctionLibrary.h"

EBTNodeResult::Type UBTTask_MBChaseWalkState::ExecuteTask(UBehaviorTreeComponent & OwnerComp, uint8 * NodeMemory)
{
	AMiniBossAIController*  AI = Cast<AMiniBossAIController>(OwnerComp.GetAIOwner());

	TArray<AMyTargetPoint*>MovePoints;

	if (AI)
	{
		AMiniBoss* MiniBoss = Cast<AMiniBoss>(AI->GetPawn());

		if (MiniBoss)
		{
			AMotionControllerCharacter* MyCharacter = Cast<AMotionControllerCharacter>(AI->BBComponent->GetValueAsObject("Player"));
			if (MyCharacter)
			{
				if (MyCharacter->AttackPoints.Num() > 0)
				{
					MovePoints = MyCharacter->AttackPoints;
					int RandomAttackPointIndex = FMath::RandRange(0, 7);

					AI->BBComponent->SetValueAsVector("MovePoint", MyCharacter->Camera->GetComponentLocation());

					if (!AI->BBComponent->GetValueAsObject("MovePoint"))
					{
						FRotator DeviceRotation;
						FVector DevicePosition;

						UHeadMountedDisplayFunctionLibrary::GetOrientationAndPosition(DeviceRotation, DevicePosition);
						//AI->BBComponent->SetValueAsObject("MovePoint", MovePoints[RandomAttackPointIndex]);
						AI->BBComponent->SetValueAsVector("MovePoint", DevicePosition);
					}
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

		/*AMotionControllerCharacter* MyCharacter = Cast<AMotionControllerCharacter>(AI->BBComponent->GetValueAsObject("Player"));

		if (MyCharacter)
		{
			FVector MovePoint = UNavigationSystem::GetRandomPointInNavigableRadius(GetWorld(), MyCharacter->Camera->GetComponentLocation(), 300.0f);
			AI->BBComponent->SetValueAsVector("MovePoint", MovePoint);
		}*/
	}
	else
	{
		return EBTNodeResult::Failed;
	}
}
