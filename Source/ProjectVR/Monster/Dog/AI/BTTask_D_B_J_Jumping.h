// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Tasks/BTTask_BlackboardBase.h"
#include "BTTask_D_B_J_Jumping.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTVR_API UBTTask_D_B_J_Jumping : public UBTTask_BlackboardBase
{
	GENERATED_BODY()
	
public:
	class AMotionControllerCharacter* MyCharacter;

	class ADogAIController* AI;
	class ADog* Dog;

	bool bIsDeath;					// �׾�����?

	bool CurrentFalling;			// ���� ����
	bool PreviousFalling;			// �� ����

	virtual void InitializeFromAsset(UBehaviorTree& Asset) override;
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
	virtual void TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;
};
