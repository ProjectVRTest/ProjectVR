// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Services/BTService_BlackboardBase.h"
#include "BTService_CheckCanAttack.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTVR_API UBTService_CheckCanAttack : public UBTService_BlackboardBase
{
	GENERATED_BODY()

	float Range;

	virtual void InitializeFromAsset(UBehaviorTree& Asset) override;
	virtual void TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;
	
	UFUNCTION()
		void SetRandomCircle(ADog* RagdollDog);

	UFUNCTION()
		void AttackableRange(AMotionControllerCharacter* MyCharacter, ADog* RagdollDog);

	UFUNCTION()
		void LeftRange(ADogAIController* AI, ADog * RagdollDog);

	UFUNCTION()
		void RightRange(ADogAIController* AI, ADog * RagdollDog);

	UFUNCTION()
		void UnAttackableRange(AMotionControllerCharacter* MyCharacter, ADog* RagdollDog);
};
