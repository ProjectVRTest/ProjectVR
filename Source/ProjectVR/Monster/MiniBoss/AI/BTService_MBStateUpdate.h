// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Services/BTService_BlackboardBase.h"
#include "BTService_MBStateUpdate.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTVR_API UBTService_MBStateUpdate : public UBTService_BlackboardBase
{
	GENERATED_BODY()
public:
	float Distance;
	virtual void TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;	
};
