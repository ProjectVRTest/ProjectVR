// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "State/DogState//DogState.h"
#include "DogAnimInstance.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTVR_API UDogAnimInstance : public UAnimInstance
{
	GENERATED_BODY()
	
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "State")
		EDogState CurrentDogState;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "State")
		EDogAnimState CurrentDogAnimState;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "JumpState")
		EDogJumpState CurrentDogJumpState;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data")
		FRotator LookAtRotator;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "JumpData")
		bool CurrentFalling;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "JumpData")
		bool PreviousFalling;

	UFUNCTION(BlueprintCallable)
		virtual void NativeInitializeAnimation() override;
	UFUNCTION(BlueprintCallable)
		virtual void NativeUpdateAnimation(float DeltaSeconds) override;

	UFUNCTION(BlueprintCallable, Category = "AI")
		void AnimNotify_JumpStart(UAnimNotify* Notify);
	
	
};
