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

	float ChangOfHP;				// HP 증감 변화량
	float ChangOfStamina;		// 스테미너 증감 변화량
	float Damage;					// 데미지

	// 플레이어 체력으로 연결 해야함//
	float CurrentHP ;
	float VirtualHP;
	float MaxHP;

	// 플레이어 스테미너로 연결 해야함//
	float CurrentStamina;
	float VirtualStamina;
	float MaxStamina;
	float PreStamina;

	/* 체력/스테미너 관련 */
	UPROPERTY(BlueprintReadWrite, Category = "Data")
		class UProgressBar* ForwardHP;								// HP바 1(앞)
	UPROPERTY(BlueprintReadWrite, Category = "Data")
		class UProgressBar* BackwardHP;								// HP바 2(잔상)
	UPROPERTY(BlueprintReadWrite, Category = "Data")
		class UProgressBar* State;										// 스테미너바

	// 함수
	UFUNCTION()
		void GetDamage(float damage);		// 데미지를 받을 때 가상HP감소와 애니메이션 실행
	UFUNCTION()
		void RecoveryHP(float recoveryHP);				// HP를 얻는 함수
	UFUNCTION()
		void UseStamina(float _useValue);			// 스테미너를 사용하는 함수
	UFUNCTION()
		void RecoveryStamina(float _useValue);		// 스테미너를 얻는 함수
	UFUNCTION()
		void AutoGainStamina();						// 자동적으로 스테미너가 채워지는 함수
	UPROPERTY()
		FTimerHandle AutoTimerHandle;			// 타이머핸들

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
