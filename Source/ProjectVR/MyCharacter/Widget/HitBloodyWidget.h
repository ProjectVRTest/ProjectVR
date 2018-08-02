// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "HitBloodyWidget.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTVR_API UHitBloodyWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	virtual void NativeConstruct() override; //생성자

	UFUNCTION()
		UWidgetAnimation* GetAnimationByName(FName AnimationName)const;

	UFUNCTION()
		bool PlayAnimationByName(FName AnimationName,
			float StartAtTime,
			int32 NumLoopsToPlay,
			EUMGSequencePlayMode::Type PlayMode,
			float PlaybackSpeed);

protected:
	TMap<FName, UWidgetAnimation*> AnimationsMap;
	class AMotionControllerCharacter* Player;

	UFUNCTION()
		void FillAnimationsMap();
	
	
};
