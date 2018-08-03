// Fill out your copyright notice in the Description page of Project Settings.

#include "BTService_NMStateUpdate.h"
#include "Monster/Normal/NormalMonsterAIController.h"
#include "Monster/Normal/NormalMonster.h"

#include "BehaviorTree/BlackboardComponent.h"

#include "MyCharacter/MotionControllerCharacter.h"
#include "Camera/CameraComponent.h"

void UBTService_NMStateUpdate::TickNode(UBehaviorTreeComponent & OwnerComp, uint8 * NodeMemory, float DeltaSeconds)
{
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);

	ANormalMonsterAIController* AI = Cast<ANormalMonsterAIController>(OwnerComp.GetAIOwner());

	if (AI)
	{
		ANormalMonster* NormalMonster = Cast<ANormalMonster>(AI->GetPawn());

		AActor* Player = Cast<AActor>(AI->BBComponent->GetValueAsObject(TEXT("Player")));

		if (NormalMonster && Player)
		{
			AMotionControllerCharacter* MyCharacter = Cast<AMotionControllerCharacter>(Player);
			Distance = FVector::Distance(NormalMonster->GetActorLocation(), MyCharacter->Camera->GetComponentLocation());
			AI->BBComponent->SetValueAsFloat("Distance", Distance);
		}
	}
}
