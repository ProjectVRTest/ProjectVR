// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Services/BTService_BlackboardBase.h"
#include "BTService_NMChaseRunCheck.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTVR_API UBTService_NMChaseRunCheck : public UBTService_BlackboardBase
{
	GENERATED_BODY()
private:
	float Distance;
	float ChaseRunTimeCount;
	virtual void InitializeFromAsset(UBehaviorTree& Asset) override;
	virtual void TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;
};
