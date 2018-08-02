// Fill out your copyright notice in the Description page of Project Settings.

#include "BTTask_RandomMovePointSelect.h"

#include "Monster/MiniBoss/MiniBoss.h"
#include "Monster/MiniBoss/MiniBossAIController.h"

#include "MyTargetPoint.h"

#include "MyCharacter/MotionControllerCharacter.h"
#include "BehaviorTree/BlackboardComponent.h"

EBTNodeResult::Type UBTTask_RandomMovePointSelect::ExecuteTask(UBehaviorTreeComponent & OwnerComp, uint8 * NodeMemory)
{
	AMiniBossAIController* AI = Cast<AMiniBossAIController>(OwnerComp.GetAIOwner());

	TArray<AMyTargetPoint*>MovePoints;

	if (AI)
	{
		AMiniBoss* MiniBoss = Cast<AMiniBoss>(AI->GetPawn());

		if (MiniBoss)
		{
			AMotionControllerCharacter* MyCharacter = Cast<AMotionControllerCharacter>(AI->BBComponent->GetValueAsObject("Player"));

			if (MyCharacter)
			{
				MovePoints = MyCharacter->AttackPoints;

				int RandomAttackPointIndex = FMath::RandRange(0, 7);

				AI->BBComponent->SetValueAsObject("MovePoint", MovePoints[RandomAttackPointIndex]);
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
