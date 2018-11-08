// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Tasks/BTTask_BlackboardBase.h"
#include "BTTask_BossUltimateAttackStart.generated.h"

/**
 *
 */
UCLASS()
class PROJECTVR_API UBTTask_BossUltimateAttackStart : public UBTTask_BlackboardBase
{
	GENERATED_BODY()
private:
	UPROPERTY()
		class ABossAIController* AI;
	UPROPERTY()
		class ABoss* Boss;
	UPROPERTY()
		FTimerHandle UltimateSpawnLoopTimer;
	UPROPERTY()
		bool EndFlag;
	UPROPERTY()
		int32 OrbMaxCount;
	UPROPERTY()
		int32 SpawnNormalMaxCount;
	UPROPERTY()
		int32 CurrentNormalMonsterCount;
	virtual void InitializeFromAsset(UBehaviorTree& Asset) override;
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory);
	virtual void TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;

	UFUNCTION()
		void UltimateLoop();
};
