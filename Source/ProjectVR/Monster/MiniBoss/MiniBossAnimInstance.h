// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "State/MiniBossState/MonsterState.h"
#include "MiniBossAnimInstance.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTVR_API UMiniBossAnimInstance : public UAnimInstance
{
	GENERATED_BODY()
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "State")
		EMonsterState CurrentState;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "State")
		EMonsterAnimState CurrentAnimState;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "State")
		EMonsterJumpState CurrentJumpState;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "State")
		EMonsterAttackState CurrentAttackState;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data")
		FRotator LookAtRotator;

	UFUNCTION(BlueprintCallable)
		virtual void NativeUpdateAnimation(float DeltaSeconds) override;
	UFUNCTION(BlueprintCallable, Category = "AI")
		void AnimNotify_JumpAttackStart(UAnimNotify* Notify);
	UFUNCTION(BlueprintCallable, Category = "AI")
		void AnimNotify_JumpAttackEnd(UAnimNotify* Notify);
};
