// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Tasks/BTTask_BlackboardBase.h"
#include "BTTask_MBBackWalkState.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTVR_API UBTTask_MBBackWalkState : public UBTTask_BlackboardBase
{
	GENERATED_BODY()
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AfterImageOption")
		float RenderTime;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AfterImageOption")
		float TotalTime;
	class AMiniBossAIController* AI;
	class AMiniBoss* MiniBoss;
	FTimerHandle AfterImageStartTimer;
	FTimerHandle AfterImageEndTimer;
	bool IsAfterEffectOn;
	int CurrentEffectNumber;
	int MaxEffectNumber;
	float SumDelta;
	float Distance;
	class UParticleSystemComponent* AfterImageEnd;

	virtual void InitializeFromAsset(UBehaviorTree& Asset) override;
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory);
	virtual void TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;
	void AfterImageOn();
	void AfterImageOff();

};
