// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "State/MiniBossState/MiniBossState.h"
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
		EMiniBossState CurrentState;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "State")
		EMiniBossAnimState CurrentAnimState;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "State")
		EMiniBossJumpState CurrentJumpState;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "State")
		EMiniBossAttackState CurrentAttackState;	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "State")
		EMiniBossShortAttackState CurrentShortAttackState;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "State")
		EMiniBossWaveAttackState CurrentWaveAttackState;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "State")
		EMiniBossComboAttackState CurrentComboAttackState;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "State")
		EMiniBossBackAttackState CurrentBackAttackState;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "State")
		EMiniBossParryingState CurrentParryingState;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Velocity")
		float Speed;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Yaw")
		float Yaw;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Yaw")
		float RotateYaw;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Attack")
		bool StabFlag;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Attack")
		bool TwoHandWidthFlag;

	UFUNCTION(BlueprintCallable)
		virtual void NativeUpdateAnimation(float DeltaSeconds) override;
	UFUNCTION(BlueprintCallable, Category = "AI")
		void AnimNotify_JumpAttackStart(UAnimNotify* Notify);
	UFUNCTION(BlueprintCallable, Category = "AI")
		void AnimNotify_JumpAttackEnd(UAnimNotify* Notify);
	UFUNCTION(BlueprintCallable, Category = "AI")
		void AnimNotify_SwordWaveSpawn(UAnimNotify* Notify);
};
