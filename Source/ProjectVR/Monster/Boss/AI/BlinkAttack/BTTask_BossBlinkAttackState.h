// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Tasks/BTTask_BlackboardBase.h"
#include "BTTask_BossBlinkAttackState.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTVR_API UBTTask_BossBlinkAttackState : public UBTTask_BlackboardBase
{
	GENERATED_BODY()
private:
	int a;
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
};
