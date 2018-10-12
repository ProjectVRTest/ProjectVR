// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Tasks/BTTask_BlackboardBase.h"
#include "State/DontMoveArcherState/DontMoveArcherState.h"
#include "BTTask_NMDMArcherSetState.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTVR_API UBTTask_NMDMArcherSetState : public UBTTask_BlackboardBase
{
	GENERATED_BODY()
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "State")
		EDontMoveArcherState SetState;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "State")
		EDontMoveArcherAnimState SetAnimState;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "State")
		EDontMoveArcherAttackState SetAttackState;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "State")
		EDontMoveArcherArcherAttackState SetArcherAttackState;

	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
};
