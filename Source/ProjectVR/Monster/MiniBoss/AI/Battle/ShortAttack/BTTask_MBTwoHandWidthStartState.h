// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Tasks/BTTask_BlackboardBase.h"
#include "BTTask_MBTwoHandWidthStartState.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTVR_API UBTTask_MBTwoHandWidthStartState : public UBTTask_BlackboardBase
{
	GENERATED_BODY()
private:
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
};
