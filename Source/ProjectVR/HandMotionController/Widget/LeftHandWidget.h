// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "LeftHandWidget.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTVR_API ULeftHandWidget : public UUserWidget
{
	GENERATED_BODY()
public:
	virtual void NativeConstruct() override;		// 생성자
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;		// 틱

	float ChangOfHP;				// HP 증감 변화량
	float ChangOfStamina;		// 스테미너 증감 변화량
	float Damage;					// 데미지

	// 플레이어 체력으로 연결 해야함//
	float CurrentHP = 1.0f;
	float VirtualHP = 1.0f;
	float MaxHP = 1.0f;

	// 플레이어 스테미너로 연결 해야함//
	float CurrentStamina = 1.0f;
	float VirtualStamina = 1.0f;
	float MaxStamina = 1.0f;

	/* 체력/스테미너 관련 */
	UPROPERTY(BlueprintReadWrite, Category = "Data")
		class UProgressBar* ForwardHP;								// HP바 1(앞)
	UPROPERTY(BlueprintReadWrite, Category = "Data")
		class UProgressBar* BackwardHP;								// HP바 2(잔상)
	UPROPERTY(BlueprintReadWrite, Category = "Data")
		class UProgressBar* State;										// 스테미너바

	/* 포션관련 */
	UPROPERTY(BlueprintReadWrite, Category = "Data")
		class UHorizontalBox* PortionBox;								// 포션 이미지를 넣을 Horizontal박스
	UPROPERTY(BlueprintReadWrite, Category = "Data")
		TArray<class UImage*> PotionArray;								// 포션이미지 배열
	UPROPERTY(BlueprintReadWrite, Category = "Data")
		int Arraysize = 5;													// 포션 이미지 갯수

	// 애니메이션 관련
	UPROPERTY(BlueprintReadWrite, Category = "Data")
		class UWidgetAnimation* WidgetAnim;						// 위젯 애니메이션(피격시 HP바 흔들거림)

	UPROPERTY()
		FTimerHandle TimerHandle;			// 타이머핸들

	UPROPERTY()
		class AMotionControllerPC* PC; //플레이어 컨트롤러
	UPROPERTY()
		class AMotionControllerCharacter* MyCharacter; //플레이어 캐릭터

	UFUNCTION()
		void AssignAnimations();			// 위젯애니메이션 찾기
	UFUNCTION()
		void StartAnimations();			// 애니메이션 실행
	UFUNCTION()
		void StopAnimations();			// 애니메이션 정지
	UFUNCTION()
		void ReceiveDamage(float _damage);		// 데미지를 받을 때 가상HP감소와 애니메이션 실행
	UFUNCTION()
		void GainHP(float _damage);				// HP를 얻는 함수
	UFUNCTION()
		void UseStamina(float _useValue);			// 스테미너를 사용하는 함수
	UFUNCTION()
		void GainStamina(float _useValue);		// 스테미너를 얻는 함수
	UFUNCTION()
		void InvisiblePotion();							// 포션 이미지 pop하는 함수
	UFUNCTION()
		void AutoGainStamina();						// 자동적으로 스테미너가 채워지는 함수
};
