// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Tasks/BTTask_BlackboardBase.h"
#include "BTTask_MBJumpAttackReadyState.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTVR_API UBTTask_MBJumpAttackReadyState : public UBTTask_BlackboardBase
{
	GENERATED_BODY()
public:
	class AMiniBoss* MiniBoss;
	FTimerHandle DashTimer;
	FTimerHandle WalkCompleteTimer;
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
	void JumpRunCheck();
};
