// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Tasks/BTTask_BlackboardBase.h"
#include "MyTargetPoint.h"
#include "BTTask_PatrolState.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTVR_API UBTTask_PatrolState : public UBTTask_BlackboardBase
{
	GENERATED_BODY()
public:
	TArray<class AMyTargetPoint*> PatrolPoints;
	class AMyTargetPoint* PatrolPoint;
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
	
	
	
};
