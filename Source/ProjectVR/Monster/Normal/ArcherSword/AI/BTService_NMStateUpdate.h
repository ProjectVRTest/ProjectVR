// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Services/BTService_BlackboardBase.h"
#include "BTService_NMStateUpdate.generated.h"

/**
 *
 */
UCLASS()
class PROJECTVR_API UBTService_NMStateUpdate : public UBTService_BlackboardBase
{
	GENERATED_BODY()
public:
	UPROPERTY()
		float Distance;
	UPROPERTY()
		bool RageAttackFlag;
	UPROPERTY()
		FTimerHandle RageAttackTimer;

	virtual void InitializeFromAsset(UBehaviorTree& Asset) override;
	virtual void TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;

	UFUNCTION()
		void RageAttackTimerCount();
	UFUNCTION()
		void SwordManLocomotion(class ANormalMonster* NormalMonster);
	UFUNCTION()
		void BowManLocomotion(class ANormalMonster* NormalMonster);
};
