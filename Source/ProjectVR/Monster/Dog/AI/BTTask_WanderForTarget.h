// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Tasks/BTTask_BlackboardBase.h"
#include "BTTask_WanderForTarget.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTVR_API UBTTask_WanderForTarget : public UBTTask_BlackboardBase
{
	GENERATED_BODY()
	
		float time;

	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
	virtual void InitializeFromAsset(UBehaviorTree& Asset) override;
	
	
};
