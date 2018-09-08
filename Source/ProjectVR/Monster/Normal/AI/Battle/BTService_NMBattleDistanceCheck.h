// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Services/BTService_BlackboardBase.h"
#include "BTService_NMBattleDistanceCheck.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTVR_API UBTService_NMBattleDistanceCheck : public UBTService_BlackboardBase
{
	GENERATED_BODY()
private:
	FVector Velocity;
	FRotator NMRotator;
	float Distance;
	virtual void TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;
};
