// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Tasks/BTTask_BlackboardBase.h"
#include "BTTask_BossParryingLoopState.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTVR_API UBTTask_BossParryingLoopState : public UBTTask_BlackboardBase
{
	GENERATED_BODY()
private:
	class ABoss* Boss;
	bool CountAttackFlag;
	FTimerHandle BossParryingTimeTimer;
public:
	virtual void InitializeFromAsset(UBehaviorTree& Asset) override;
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory);
	virtual void TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;
	UFUNCTION()
		void CountAttack();
};
