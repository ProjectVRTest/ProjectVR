// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Services/BTService_BlackboardBase.h"
#include "BTService_DistanceCalculation.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTVR_API UBTService_DistanceCalculation : public UBTService_BlackboardBase
{
	GENERATED_BODY()

		float Distance;
	virtual void InitializeFromAsset(UBehaviorTree& Asset) override;
	virtual void TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;
	
	
};
