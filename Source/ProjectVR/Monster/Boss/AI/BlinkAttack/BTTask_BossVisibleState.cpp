// Fill out your copyright notice in the Description page of Project Settings.

#include "BTTask_BossVisibleState.h"
#include "Headers/BossAIHeader.h"
#include "Components/SkeletalMeshComponent.h"

EBTNodeResult::Type UBTTask_BossVisibleState::ExecuteTask(UBehaviorTreeComponent & OwnerComp, uint8 * NodeMemory)
{
	ABossAIController* AI = Cast<ABossAIController>(OwnerComp.GetAIOwner());

	if (AI)
	{
		Boss = Cast<ABoss>(AI->GetPawn());

		if (Boss)
		{
			Boss->SetActorLocation(AI->BBComponent->GetValueAsVector("TelepoteLocation"));
			Boss->GetMesh()->SetMaterial(0, Boss->DefaultBodyMaterials);
			Boss->GetMesh()->SetMaterial(1, Boss->DefaultClothMaterials);
			Boss->GetMesh()->SetCollisionProfileName("CharacterMesh");
			Boss->CurrentBlinkAttackState = EBossBlinkAttackState::BlinkAttack;
		}		
	}
	return EBTNodeResult::Succeeded;
}


