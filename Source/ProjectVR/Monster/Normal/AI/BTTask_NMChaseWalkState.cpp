// Fill out your copyright notice in the Description page of Project Settings.

#include "BTTask_NMChaseWalkState.h"
#include "Headers/NormalMonsterAIHeader.h"
#include "MyCharacter/MotionControllerCharacter.h"
#include "AI/Navigation/NavigationSystem.h"

#include "Camera/CameraComponent.h"

EBTNodeResult::Type UBTTask_NMChaseWalkState::ExecuteTask(UBehaviorTreeComponent & OwnerComp, uint8 * NodeMemory)
{
	AI = Cast<ANormalMonsterAIController>(OwnerComp.GetAIOwner());

	if (AI)
	{
		MyCharacter = Cast<AMotionControllerCharacter>(AI->BBComponent->GetValueAsObject("Player"));
		
		if (MyCharacter)
		{
			FVector MovePoint = UNavigationSystem::GetRandomPointInNavigableRadius(GetWorld(), MyCharacter->Camera->GetComponentLocation(), 300.0f);
			AI->BBComponent->SetValueAsVector("MovePoint", MovePoint);
		}
	}
	return EBTNodeResult::Succeeded;
}
