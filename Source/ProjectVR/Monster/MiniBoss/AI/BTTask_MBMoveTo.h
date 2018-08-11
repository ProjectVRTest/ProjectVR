// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Tasks/BTTask_BlackboardBase.h"
#include "BTTask_MBMoveTo.generated.h"

/**
 *
 */
UCLASS()
class PROJECTVR_API UBTTask_MBMoveTo : public UBTTask_BlackboardBase
{
	GENERATED_BODY()
public:
	FTimerHandle WalkTimer;
	virtual void InitializeFromAsset(UBehaviorTree& Asset) override;
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
	virtual void TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)override;
	bool ExitFlag;
	void DelayStart();
};
