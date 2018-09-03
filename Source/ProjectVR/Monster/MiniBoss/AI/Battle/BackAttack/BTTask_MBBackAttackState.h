// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Tasks/BTTask_BlackboardBase.h"
#include "BTTask_MBBackAttackState.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTVR_API UBTTask_MBBackAttackState : public UBTTask_BlackboardBase
{
	GENERATED_BODY()
private:
	int a;
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory);	
};
