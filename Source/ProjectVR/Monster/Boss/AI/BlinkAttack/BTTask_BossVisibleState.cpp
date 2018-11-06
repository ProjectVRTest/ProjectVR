// Fill out your copyright notice in the Description page of Project Settings.

#include "BTTask_BossVisibleState.h"
#include "Headers/BossAIHeader.h"
#include "Components/SkeletalMeshComponent.h"
#include "GameFramework/CharacterMovementComponent.h"

EBTNodeResult::Type UBTTask_BossVisibleState::ExecuteTask(UBehaviorTreeComponent & OwnerComp, uint8 * NodeMemory)
{
	ABossAIController* AI = Cast<ABossAIController>(OwnerComp.GetAIOwner());

	if (AI)
	{
		Boss = Cast<ABoss>(AI->GetPawn());

		if (Boss)
		{
			//Boss->GetCharacterMovement()->GravityScale = 0;
			Boss->SetActorLocation(AI->BBComponent->GetValueAsVector("TeleportLocation"));
			Boss->GetMesh()->SetMaterial(0, Boss->DefaultBodyMaterials);
			Boss->GetMesh()->SetMaterial(1, Boss->DefaultClothMaterials);
			Boss->GetMesh()->SetCollisionProfileName("CharacterMesh");

			switch (Boss->CurrentBattleState)
			{
			case EBossBattleState::AddAttack:
				Boss->CurrentBlinkAttackState = EBossBlinkAttackState::BlinkAttack;
				break;
			case EBossBattleState::BattleWatch:
				Boss->CurrentBlinkAttackState = EBossBlinkAttackState::Idle;
				Boss->CurrentBattleState = EBossBattleState::AttackReady;		
				break;
			case EBossBattleState::UltimateAttack:
				Boss->CurrentUltimateAttackState = EBossUltimateAttackState::ManyOrbCreateReady;
				break;
			}
		}		
	}
	return EBTNodeResult::Succeeded;
}


