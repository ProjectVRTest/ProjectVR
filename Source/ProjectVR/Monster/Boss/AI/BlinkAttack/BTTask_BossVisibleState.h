// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Tasks/BTTask_BlackboardBase.h"
#include "BTTask_BossVisibleState.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTVR_API UBTTask_BossVisibleState : public UBTTask_BlackboardBase
{
	GENERATED_BODY()
public:
	class ABoss* Boss;
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory);
};
