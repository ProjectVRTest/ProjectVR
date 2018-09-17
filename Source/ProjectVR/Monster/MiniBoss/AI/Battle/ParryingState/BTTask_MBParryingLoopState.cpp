// Fill out your copyright notice in the Description page of Project Settings.

#include "BTTask_MBParryingLoopState.h"
#include "Headers/MiniBossAIHeader.h"

void UBTTask_MBParryingLoopState::InitializeFromAsset(UBehaviorTree & Asset)
{
	Super::InitializeFromAsset(Asset);

	bNotifyTick = true;
	CountAttackFlag = false;
}

EBTNodeResult::Type UBTTask_MBParryingLoopState::ExecuteTask(UBehaviorTreeComponent & OwnerComp, uint8 * NodeMemory)
{
	AMiniBossAIController* AI = Cast<AMiniBossAIController>(OwnerComp.GetAIOwner());

	if (AI)
	{
		AMiniBoss* MiniBoss = Cast<AMiniBoss>(AI->GetPawn());

		if (MiniBoss)
		{
			GetWorld()->GetTimerManager().SetTimer(ParryingTimeTimer, this, &UBTTask_MBParryingLoopState::CountAttack,2.0f);
		}
	}
	return EBTNodeResult::InProgress;
}

void UBTTask_MBParryingLoopState::TickTask(UBehaviorTreeComponent & OwnerComp, uint8 * NodeMemory, float DeltaSeconds)
{
	Super::TickTask(OwnerComp, NodeMemory, DeltaSeconds);

}

void UBTTask_MBParryingLoopState::CountAttack()
{

}
