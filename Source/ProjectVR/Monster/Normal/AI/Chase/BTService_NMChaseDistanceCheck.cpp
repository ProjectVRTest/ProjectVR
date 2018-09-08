// Fill out your copyright notice in the Description page of Project Settings.

#include "BTService_NMChaseDistanceCheck.h"
#include "Headers/NormalMonsterAIHeader.h"

void UBTService_NMChaseDistanceCheck::InitializeFromAsset(UBehaviorTree & Asset)
{
	Super::InitializeFromAsset(Asset);
	ChaseWalkTimeCount = 0;
	ChaseRunTimerCount = 0;
}

void UBTService_NMChaseDistanceCheck::TickNode(UBehaviorTreeComponent & OwnerComp, uint8 * NodeMemory, float DeltaSeconds)
{
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);

	FName DistanceBlackBoardKey = GetSelectedBlackboardKey();

	ANormalMonsterAIController* AI = Cast<ANormalMonsterAIController>(OwnerComp.GetAIOwner());

	if (AI)
	{
		Distance = AI->BBComponent->GetValueAsFloat(DistanceBlackBoardKey);

		ANormalMonster* NormalMonster = Cast<ANormalMonster>(AI->GetPawn());

		if (NormalMonster)
		{		
			switch (NormalMonster->CurrentAnimState)
			{
			case ENormalMonsterAnimState::Wait:
				if (Distance >= 400.0f)
				{
					NormalMonster->CurrentAnimState = ENormalMonsterAnimState::Walk;
				}
				else if (Distance < 400.0f)
				{
					NormalMonster->CurrentAnimState = ENormalMonsterAnimState::AttackReady;
					NormalMonster->CurrentState = ENormalMonsterState::Battle;
				}
				break;
			case ENormalMonsterAnimState::Walk:
				if (Distance < 400.0f)
				{
					NormalMonster->CurrentAnimState = ENormalMonsterAnimState::AttackReady;
					NormalMonster->CurrentState = ENormalMonsterState::Battle;
				}
				break;
			case ENormalMonsterAnimState::Run:
				if (Distance < 400.0f)
				{
					NormalMonster->CurrentAnimState = ENormalMonsterAnimState::AttackReady;
					NormalMonster->CurrentState = ENormalMonsterState::Battle;
				}
				break;
			case ENormalMonsterAnimState::RageRun:
				if (Distance <= 400.0f)
				{
					NormalMonster->CurrentAnimState = ENormalMonsterAnimState::RageAttack;
				}
				break;
			}
		}
	}
}
