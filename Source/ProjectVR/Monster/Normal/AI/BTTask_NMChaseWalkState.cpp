// Fill out your copyright notice in the Description page of Project Settings.

#include "BTTask_NMChaseWalkState.h"
#include "Headers/NormalMonsterAIHeader.h"
#include "MyCharacter/MotionControllerCharacter.h"
#include "AI/Navigation/NavigationSystem.h"

EBTNodeResult::Type UBTTask_NMChaseWalkState::ExecuteTask(UBehaviorTreeComponent & OwnerComp, uint8 * NodeMemory)
{
	ANormalMonsterAIController* AI = Cast<ANormalMonsterAIController>(OwnerComp.GetAIOwner());

	if (AI)
	{
		AMotionControllerCharacter* MyCharacter = Cast<AMotionControllerCharacter>(AI->BBComponent->GetValueAsObject("Player"));
		
		if (MyCharacter)
		{
			UE_LOG(LogClass, Warning, TEXT("Walk"));
			AI->MoveToActor(MyCharacter,300.0f);
			
			FVector MovePoint =UNavigationSystem::GetRandomPointInNavigableRadius(GetWorld(), MyCharacter->GetActorLocation(), 300.0f);

			AI->BBComponent->SetValueAsVector("MovePoint", MovePoint);
			AI->MoveToLocation(MovePoint, 300.0f);

		}
	}

	return EBTNodeResult::InProgress;
}
