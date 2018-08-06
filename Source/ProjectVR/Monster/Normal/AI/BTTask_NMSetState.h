// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Tasks/BTTask_BlackboardBase.h"
#include "State/NormalMonsterState/NormalMonsterState.h"
#include "BTTask_NMSetState.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTVR_API UBTTask_NMSetState : public UBTTask_BlackboardBase
{
	GENERATED_BODY()
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "State")
		ENormalMonsterState SetState;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "State")
		ENormalMonsterAnimState SetAnimState;
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
	
	
	
};
