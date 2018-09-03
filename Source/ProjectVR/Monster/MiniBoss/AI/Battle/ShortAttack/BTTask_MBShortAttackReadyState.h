// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Tasks/BTTask_BlackboardBase.h"
#include "BTTask_MBShortAttackReadyState.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTVR_API UBTTask_MBShortAttackReadyState : public UBTTask_BlackboardBase
{
	GENERATED_BODY()
private:
	class AMiniBossAIController* AI;
	class AMiniBoss* MiniBoss;
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory);	
};
