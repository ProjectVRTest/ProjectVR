// Fill out your copyright notice in the Description page of Project Settings.

#include "MotionControllerPC.h"
#include "UObject/ConstructorHelpers.h"
#include "Haptics/HapticFeedbackEffect_Base.h"

AMotionControllerPC::AMotionControllerPC()
{
	static ConstructorHelpers::FObjectFinder<UHapticFeedbackEffect_Base>HandHapticEffect(TEXT("HapticFeedbackEffect_Curve'/Game/Assets/CharacterEquipment/MyCharacter/Hand/HandHaptics.HandHaptics'"));

	if (HandHapticEffect.Succeeded())
	{
		HapticEffect = HandHapticEffect.Object;
	}
}

void AMotionControllerPC::RumbleHandController(EControllerHand& Hand, float& Intensity)
{
	PlayHapticEffect(HapticEffect, Hand, Intensity);
}

