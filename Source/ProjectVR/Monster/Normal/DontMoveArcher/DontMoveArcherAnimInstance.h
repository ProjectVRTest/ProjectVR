// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "State/DontMoveArcherState/DontMoveArcherState.h"
#include "DontMoveArcherAnimInstance.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTVR_API UDontMoveArcherAnimInstance : public UAnimInstance
{
	GENERATED_BODY()
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "State")
		EDontMoveArcherState CurrentState;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "State")
		EDontMoveArcherAnimState CurrentAnimState;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "State")
		EDontMoveArcherAttackState CurrentAttackState;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "State")
		EDontMoveArcherArcherAttackState CurrentArcherAttackState;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Pitch")
		float Pitch;

	UFUNCTION(BlueprintCallable)
		virtual void NativeUpdateAnimation(float DeltaSeconds) override;
	UFUNCTION(BlueprintCallable, Category = "Bow")
		void AnimNotify_ArrowSpawn(UAnimNotify* Notify);
	UFUNCTION(BlueprintCallable, Category = "Bow")
		void AnimNotify_ArrowDestroy(UAnimNotify* Notify);
};
