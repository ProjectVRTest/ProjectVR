// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Tasks/BTTask_BlackboardBase.h"
#include "BTTask_NMDeadState.generated.h"

/**
 *
 */
UCLASS()
class PROJECTVR_API UBTTask_NMDeadState : public UBTTask_BlackboardBase
{
	GENERATED_BODY()
public:
	class ANormalMonsterAIController* AI;
	FTimerHandle DestroyTimer;
	FTimerHandle DestroyRenderTimer;
	float DeathMaterialsValue;
	class ANormalMonster*	NormalMonster;

	virtual void InitializeFromAsset(UBehaviorTree& Asset) override;
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
	//virtual void TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;
	UFUNCTION()
		void Destroy();
	UFUNCTION()
		void DestroyRender();
};
