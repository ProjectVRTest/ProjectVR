// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "MotionControllerPC.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTVR_API AMotionControllerPC : public APlayerController
{
	GENERATED_BODY()
public:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Haptic")
		class UHapticFeedbackEffect_Base* HapticEffect;

	AMotionControllerPC();

	UFUNCTION()
		void RumbleHandController(EControllerHand& Hand,float& Intensity);
};
