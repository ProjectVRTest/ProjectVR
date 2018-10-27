// Fill out your copyright notice in the Description page of Project Settings.

#include "BTService_NMChaseRunCheck.h"
#include "Headers/NormalMonsterAIHeader.h"


void UBTService_NMChaseRunCheck::InitializeFromAsset(UBehaviorTree & Asset)
{
	Super::InitializeFromAsset(Asset);
	ChaseRunTimeCount = 0;
}

void UBTService_NMChaseRunCheck::TickNode(UBehaviorTreeComponent & OwnerComp, uint8 * NodeMemory, float DeltaSeconds)
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
			ChaseRunTimeCount+=0.5f;

			if (ChaseRunTimeCount >= 3.5f)
			{
				ChaseRunTimeCount = 0;
				NormalMonster->CurrentAnimState = ENormalMonsterAnimState::RageRun;
			}
		}
	}
}
