// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Tasks/BTTask_BlackboardBase.h"
#include "State/MiniBossState/MiniBossState.h"
#include "BTTask_MBSetState.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTVR_API UBTTask_MBSetState : public UBTTask_BlackboardBase
{
	GENERATED_BODY()
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "State")
		EMiniBossState SetState;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "State")
		EMiniBossAnimState SetAnimState;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "State")
		EMiniBossJumpState SetJumpAnimState;
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
	
};
