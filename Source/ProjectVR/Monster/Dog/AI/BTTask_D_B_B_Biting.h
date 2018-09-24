// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Tasks/BTTask_BlackboardBase.h"
#include "BTTask_D_B_B_Biting.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTVR_API UBTTask_D_B_B_Biting : public UBTTask_BlackboardBase
{
	GENERATED_BODY()
	
public:
	class ADogAIController* AI;
	class ADog* Dog;
	class ARightHandMotionController* RightController;
	class AMotionControllerCharacter* Player;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stack")
		int MaxStack;

	virtual void InitializeFromAsset(UBehaviorTree& Asset) override;
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
	virtual void TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;
};
