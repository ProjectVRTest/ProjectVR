// Fill out your copyright notice in the Description page of Project Settings.

#include "BTTask_MBMoveTo.h"
#include "Headers/MiniBossAIHeader.h"
#include "Navigation/PathFollowingComponent.h"

void UBTTask_MBMoveTo::InitializeFromAsset(UBehaviorTree & Asset)
{
	Super::InitializeFromAsset(Asset);

	bNotifyTick = true;
}

EBTNodeResult::Type UBTTask_MBMoveTo::ExecuteTask(UBehaviorTreeComponent & OwnerComp, uint8 * NodeMemory)
{
	ExitFlag = false;
	return EBTNodeResult::InProgress;
}

void UBTTask_MBMoveTo::TickTask(UBehaviorTreeComponent & OwnerComp, uint8 * NodeMemory, float DeltaSeconds)
{
	Super::TickTask(OwnerComp, NodeMemory, DeltaSeconds);

	AMiniBossAIController* PC = Cast<AMiniBossAIController>(OwnerComp.GetAIOwner());

	if (PC)
	{
		AActor* Goal = Cast<AActor>(PC->BBComponent->GetValueAsObject("MovePoint"));
		EPathFollowingRequestResult::Type GoalResult=PC->MoveToActor(Goal, 100.0f,true,true,true,PC->GetDefaultNavigationFilterClass(),false);
		switch (GoalResult)
		{
		case EPathFollowingRequestResult::AlreadyAtGoal:
			GLog->Log(FString::Printf(TEXT("골지점 도착")));
			FinishLatentTask(OwnerComp, EBTNodeResult::Failed);
			break;
		case EPathFollowingRequestResult::Failed:
			break;
		case EPathFollowingRequestResult::RequestSuccessful:
			//GLog->Log(FString::Printf(TEXT("리퀘스트성공")));
			GetWorld()->GetTimerManager().SetTimer(WalkTimer, this, &UBTTask_MBMoveTo::DelayStart, 0.2f);
			break;
		}
	}
	if (ExitFlag)
	{
		FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
	}
}

void UBTTask_MBMoveTo::DelayStart()
{
	ExitFlag = true;
}



