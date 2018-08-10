// Fill out your copyright notice in the Description page of Project Settings.

#include "BTService_MBChaseDistanceCheck.h"
#include "Headers/MiniBossAIHeader.h"

void UBTService_MBChaseDistanceCheck::TickNode(UBehaviorTreeComponent & OwnerComp, uint8 * NodeMemory, float DeltaSeconds)
{
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);

	FName DistanceBlackBoardKey = GetSelectedBlackboardKey();

	AMiniBossAIController* AI = Cast<AMiniBossAIController>(OwnerComp.GetAIOwner());

	if (AI)
	{
		Distance = AI->BBComponent->GetValueAsFloat(DistanceBlackBoardKey);

		AMiniBoss* MiniBoss = Cast<AMiniBoss>(AI->GetPawn());

		if (MiniBoss)
		{
			if (Distance > 1000.0f)
			{
				UE_LOG(LogClass, Warning, TEXT("점프 애드 범위 진입"));
			}
			else if(Distance <1000.0f && Distance >400.0f)
			{
				UE_LOG(LogClass, Warning, TEXT("대쉬 애드 범위 진입"));
			}
			else if (Distance < 300.0f)
			{
				UE_LOG(LogClass, Warning, TEXT("대치 접근 범위 진입"));
			}
		}
	}
}
