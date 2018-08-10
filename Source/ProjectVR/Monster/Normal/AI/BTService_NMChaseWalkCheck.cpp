// Fill out your copyright notice in the Description page of Project Settings.

#include "BTService_NMChaseWalkCheck.h"
#include "Headers/NormalMonsterAIHeader.h"

void UBTService_NMChaseWalkCheck::InitializeFromAsset(UBehaviorTree & Asset)
{
	Super::InitializeFromAsset(Asset);

}

void UBTService_NMChaseWalkCheck::TickNode(UBehaviorTreeComponent & OwnerComp, uint8 * NodeMemory, float DeltaSeconds)
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
			if (Distance > 500.0f)
			{
				UE_LOG(LogClass, Warning, TEXT("거리확인 진입"));
				//NormalMonster->CurrentAnimState = ENormalMonsterAnimState::Run;
			}
		}
	}
}
