// Fill out your copyright notice in the Description page of Project Settings.

#include "BTService_MBChaseDashCountCheck.h"
#include "Headers/MiniBossAIHeader.h"

void UBTService_MBChaseDashCountCheck::InitializeFromAsset(UBehaviorTree & Asset)
{
	Super::InitializeFromAsset(Asset);

	DashCount = 0;
}

void UBTService_MBChaseDashCountCheck::TickNode(UBehaviorTreeComponent & OwnerComp, uint8 * NodeMemory, float DeltaSeconds)
{
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);

	AMiniBossAIController* AI = Cast<AMiniBossAIController>(OwnerComp.GetAIOwner());

	if (AI)
	{
		AI->BBComponent->SetValueAsInt(TEXT("DashCount"), DashCount);
		DashCount++;

		int DashRandom = FMath::RandRange(1, 20);

		//GLog->Log(FString::Printf(TEXT("DashRandom : %d"), DashRandom));
		if (DashRandom < 2)
		{
			AMiniBoss* MiniBoss = Cast<AMiniBoss>(AI->GetPawn());

			if (MiniBoss)
			{
				MiniBoss->CurrentAnimState = EMiniBossAnimState::DashAttack;
			}
		}
	}
}
