// Fill out your copyright notice in the Description page of Project Settings.

#include "BTTask_MBChaseWalkState.h"
#include "Headers/MiniBossAIHeader.h"

#include "AI/Navigation/NavigationSystem.h"
#include "MyTargetPoint.h"

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

					if (!AI->BBComponent->GetValueAsObject("MovePoint"))
					{
						AI->BBComponent->SetValueAsObject("MovePoint", MovePoints[RandomAttackPointIndex]);
					}
					return EBTNodeResult::Succeeded;
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
