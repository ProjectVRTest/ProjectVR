// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Tasks/BTTask_BlackboardBase.h"
#include "State/BossState/BossState.h"
#include "BTTask_BossSetState.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTVR_API UBTTask_BossSetState : public UBTTask_BlackboardBase
{
	GENERATED_BODY()
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "State")
		EBossState SetState;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "State")
		EBossBattleState SetBattleState;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "State")
		EBossBlinkAttackState SetBlinkAttackState;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "State")
		EBossLongAttackState SetLongAttackState;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "State")
		EBossCloseAttackState SetCloseAttackState;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "State")
		EBossBattleWatchState SetBattleWatchState;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "State")
		EBossConfrontationState SetConfrontationState;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "State")
		EBossUltimateAttackState SetUltimateAttackState; 
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "State")
		EBossWaveAttackState SetWaveAttackState;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "State")
		EBossUpDownAttackState SetUpDownAttackState;
private:
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory);

};
