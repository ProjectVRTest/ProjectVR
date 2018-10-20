// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Tasks/BTTask_BlackboardBase.h"
#include "BTTask_MBBackWalkState.generated.h"

/**
 *
 */
UCLASS()
class PROJECTVR_API UBTTask_MBBackWalkState : public UBTTask_BlackboardBase
{
	GENERATED_BODY()
public:
	FTimerHandle ToForceStateTimer;
	FTimerHandle InVisibleTimer;
	class AMiniBossAIController* AI;
	class AMiniBoss* MiniBoss;
	float InVisibleValue;
	bool ExitFlag;
	virtual void InitializeFromAsset(UBehaviorTree& Asset) override;
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory);
	virtual void TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;
	UFUNCTION()
		void ToForceState();
	UFUNCTION()
		void InVisible();
};
