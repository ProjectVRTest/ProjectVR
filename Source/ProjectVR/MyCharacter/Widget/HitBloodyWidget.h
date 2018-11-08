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
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;		// 틱

	UPROPERTY()
		class AMotionControllerPC* PC;				   // 게임모드의 플레이어컨트롤러 클래스
	UPROPERTY()
		class AMotionControllerCharacter* MyCharacter; //플레이어 캐릭터. 매틱마다 체력과 스테미너를 불러옴 -> 나중에 메모리사용량 줄일거임


	// 애니메이션 
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
