// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Tasks/BTTask_BlackboardBase.h"
#include "BTTask_MBAttackReadyState.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTVR_API UBTTask_MBAttackReadyState : public UBTTask_BlackboardBase
{
	GENERATED_BODY()
private:
	class AMiniBoss* MiniBoss;
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory);

	UFUNCTION()
		float WallCheckDistance();
};
