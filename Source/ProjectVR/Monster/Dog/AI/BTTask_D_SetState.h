// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Tasks/BTTask_BlackboardBase.h"
#include "State/DogState/DogState.h"
#include "BTTask_D_SetState.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTVR_API UBTTask_D_SetState : public UBTTask_BlackboardBase
{
	GENERATED_BODY()
	
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "State")
		EDogState DogState;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "State")
		EDogAnimState DogAnimState;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "State")
		EDogBattleState DogBattleState;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "BattleState")
		EDogCircleState DogCircleState;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "BattleState")
		EDogJumpState DogJumpState;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "BattleState")
		EDogAirState DogAirState;

	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
	
	
};
