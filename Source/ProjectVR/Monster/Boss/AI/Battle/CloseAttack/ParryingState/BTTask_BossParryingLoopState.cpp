// Fill out your copyright notice in the Description page of Project Settings.

#include "BTTask_BossParryingLoopState.h"
#include "Headers/BossAIHeader.h"

void UBTTask_BossParryingLoopState::InitializeFromAsset(UBehaviorTree & Asset)
{
	Super::InitializeFromAsset(Asset);

	bNotifyTick = true;
}

EBTNodeResult::Type UBTTask_BossParryingLoopState::ExecuteTask(UBehaviorTreeComponent & OwnerComp, uint8 * NodeMemory)
{
	ABossAIController* AI = Cast<ABossAIController>(OwnerComp.GetAIOwner());

	if (AI)
	{
		CountAttackFlag = false;

		Boss = Cast<ABoss>(AI->GetPawn());

		if (Boss)
		{
			GetWorld()->GetTimerManager().SetTimer(BossParryingTimeTimer, this, &UBTTask_BossParryingLoopState::CountAttack, 2.0f);
		}
	}

	return EBTNodeResult::InProgress;
}

void UBTTask_BossParryingLoopState::TickTask(UBehaviorTreeComponent & OwnerComp, uint8 * NodeMemory, float DeltaSeconds)
{
	Super::TickTask(OwnerComp, NodeMemory, DeltaSeconds);


	if (Boss)
	{
		if (Boss->ParryingPointMaxCount == Boss->ParryingPointCount)
		{
			Boss->ParryingPointCount = 0;
			Boss->CurrentParryingState = EBossParryingState::ParryingSuccess;
			FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
		}

		if (CountAttackFlag)
		{
			FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
		}
	}
}

void UBTTask_BossParryingLoopState::CountAttack()
{
	if (Boss)
	{
		Boss->CurrentParryingState = EBossParryingState::CounterAttack;
		CountAttackFlag = true;
	}
}
