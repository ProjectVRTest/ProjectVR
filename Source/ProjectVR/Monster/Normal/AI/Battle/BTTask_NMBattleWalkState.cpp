// Fill out your copyright notice in the Description page of Project Settings.

#include "BTTask_NMBattleWalkState.h"
#include "Headers/NormalMonsterAIHeader.h"

void UBTTask_NMBattleWalkState::InitializeFromAsset(UBehaviorTree & Asset)
{
	Super::InitializeFromAsset(Asset);
	bNotifyTick = true;
	RandomMove = 0;
	RandomMovePattern = 0;
}

EBTNodeResult::Type UBTTask_NMBattleWalkState::ExecuteTask(UBehaviorTreeComponent & OwnerComp, uint8 * NodeMemory)
{
	AI = Cast<ANormalMonsterAIController>(OwnerComp.GetAIOwner());

	if (AI)
	{
		NormalMonster = Cast<ANormalMonster>(AI->GetPawn());
		RandomMovePattern = FMath::RandRange(1, 15);
		RandomMove = FMath::RandRange(0, 1);
	}
	return EBTNodeResult::InProgress;
}

void UBTTask_NMBattleWalkState::TickTask(UBehaviorTreeComponent & OwnerComp, uint8 * NodeMemory, float DeltaSeconds)
{
	Super::TickTask(OwnerComp, NodeMemory, DeltaSeconds);

	if (NormalMonster)
	{
		GLog->Log(FString::Printf(TEXT("RandomMovePattern : %d "), RandomMovePattern));

		float Distance = AI->BBComponent->GetValueAsFloat("Distance");

		if (RandomMovePattern > 0 && RandomMovePattern < 6)
		{
			switch (RandomMove)
			{
			case 0:
				NormalMonster->AddMovementInput(NormalMonster->GetActorRightVector(), 1.5f);
				NormalMonster->AddMovementInput(NormalMonster->GetActorForwardVector(), 1.5f);
				break;
			case 1:
				NormalMonster->AddMovementInput(NormalMonster->GetActorRightVector(), -1.5f);
				NormalMonster->AddMovementInput(NormalMonster->GetActorForwardVector(), 1.5f);
				break;
			}
		}
		else if (RandomMovePattern > 5 && RandomMovePattern < 10)
		{
			NormalMonster->CurrentAnimState = ENormalMonsterAnimState::Wait;
		}
		else if (RandomMovePattern > 9 && RandomMovePattern <= 15)
		{
			NormalMonster->CurrentState = ENormalMonsterState::Chase;
			NormalMonster->CurrentAnimState = ENormalMonsterAnimState::Wait;
		}
	}
}
