// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Tasks/BTTask_BlackboardBase.h"
#include "BTTask_MBJumpState.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTVR_API UBTTask_MBJumpState : public UBTTask_BlackboardBase
{
	GENERATED_BODY()
public:
	class AMiniBoss* MiniBoss;
	bool CurrentFalling;
	bool PreviousFalling;
	virtual void InitializeFromAsset(UBehaviorTree& Asset) override;
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
	virtual void TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;
};
