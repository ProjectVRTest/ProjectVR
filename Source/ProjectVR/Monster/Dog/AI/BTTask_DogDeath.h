// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Tasks/BTTask_BlackboardBase.h"
#include "BTTask_DogDeath.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTVR_API UBTTask_DogDeath : public UBTTask_BlackboardBase
{
	GENERATED_BODY()
	
	float OpacityValue;
	FTimerHandle DetsroyTimer;

	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

	// 나중에 효과를 줄것임
	
};
