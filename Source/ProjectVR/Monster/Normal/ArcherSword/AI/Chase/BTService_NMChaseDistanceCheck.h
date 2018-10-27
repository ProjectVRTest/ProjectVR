// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Services/BTService_BlackboardBase.h"
#include "BTService_NMChaseDistanceCheck.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTVR_API UBTService_NMChaseDistanceCheck : public UBTService_BlackboardBase
{
	GENERATED_BODY()
private:
	UPROPERTY()
		float Distance;
	UPROPERTY()
		float ChaseWalkTimeCount;
	UPROPERTY()
		float ChaseRunTimerCount;
	UPROPERTY()
		bool RageAttackFlag;

	class ANormalMonster* NormalMonster;
	FTimerHandle RageAttackTimer;
	
	virtual void InitializeFromAsset(UBehaviorTree& Asset) override;
	virtual void TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;
	void RageAttackTimerCount();
};
