// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "State/DontMoveArcherState/DontMoveArcherState.h"
#include "DontMoveArcherBowAnimInstance.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTVR_API UDontMoveArcherBowAnimInstance : public UAnimInstance
{
	GENERATED_BODY()
public:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "State")
		EDontMoveArcherAttackState CurrentAttackState;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "State")
		EDontMoveArcherArcherAttackState CurrentArcherAttackState;
	
	UFUNCTION(BlueprintCallable)
		virtual void NativeUpdateAnimation(float DeltaSeconds) override;	
};
