// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Tasks/BTTask_BlackboardBase.h"
#include "BTTask_NMIdleState.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTVR_API UBTTask_NMIdleState : public UBTTask_BlackboardBase
{
	GENERATED_BODY()
public:
	FTimerHandle PatrolOnTimer;
	class ANormalMonster* NormalMonster;
	virtual void InitializeFromAsset(UBehaviorTree& Asset) override;
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
	void PatrolStart();
};
