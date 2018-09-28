// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Tasks/BTTask_BlackboardBase.h"
#include "BTTask_MBRotateState.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTVR_API UBTTask_MBRotateState : public UBTTask_BlackboardBase
{
	GENERATED_BODY()
private:
	class AMiniBoss * MiniBoss;
	class AMotionControllerCharacter* MyCharacter;
	FRotator LookAt;
	virtual void InitializeFromAsset(UBehaviorTree& Asset) override;
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory);
	virtual void TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;
};
