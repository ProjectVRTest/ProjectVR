// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "State/NormalMonsterState/NormalMonsterState.h"
#include "NormalMonsterAnimInstance.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTVR_API UNormalMonsterAnimInstance : public UAnimInstance
{
	GENERATED_BODY()
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "State")
		ENormalMonsterState CurrentState;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "State")
		ENormalMonsterAnimState CurrentAnimState;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "State")
		ENormalMonsterAttackState CurrentAttackState;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "State")
		ENormalMonsterIdleState CurrentIdleState;
	
	UFUNCTION(BlueprintCallable)
		virtual void NativeUpdateAnimation(float DeltaSeconds) override;	
};
