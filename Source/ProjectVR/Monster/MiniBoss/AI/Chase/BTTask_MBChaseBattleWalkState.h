// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Tasks/BTTask_BlackboardBase.h"
#include "BTTask_MBChaseBattleWalkState.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTVR_API UBTTask_MBChaseBattleWalkState : public UBTTask_BlackboardBase
{
	GENERATED_BODY()
public:
	int RandomMove,MoveCount;
	int RandomChaseValue;
	class AMiniBossAIController* AI;
	class AMiniBoss* MiniBoss;

	virtual void InitializeFromAsset(UBehaviorTree& Asset) override;
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory);
	virtual void TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;	
};
