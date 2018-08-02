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
	virtual void NativeConstruct() override;		// ������
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;		// ƽ

	float ChangOfHP;				// HP ���� ��ȭ��
	float ChangOfStamina;		// ���׹̳� ���� ��ȭ��
	float Damage;					// ������

	// �÷��̾� ü������ ���� �ؾ���//
	float CurrentHP = 1.0f;
	float VirtualHP = 1.0f;
	float MaxHP = 1.0f;
	// �÷��̾� ���׹̳ʷ� ���� �ؾ���//
	float CurrentStamina = 1.0f;
	float VirtualStamina = 1.0f;
	float MaxStamina = 1.0f;

	/* ü��/���׹̳� ���� */
	UPROPERTY(BlueprintReadWrite, Category = "Data")
		class UProgressBar* ForwardHP;								// HP�� 1(��)
	UPROPERTY(BlueprintReadWrite, Category = "Data")
		class UProgressBar* BackwardHP;								// HP�� 2(�ܻ�)
	UPROPERTY(BlueprintReadWrite, Category = "Data")
		class UProgressBar* State;										// ���׹̳ʹ�

	/* ���ǰ��� */
	UPROPERTY(BlueprintReadWrite, Category = "Data")
		class UHorizontalBox* PortionBox;								// ���� �̹����� ���� Horizontal�ڽ�
	UPROPERTY(BlueprintReadWrite, Category = "Data")
		TArray<class UImage*> PotionArray;								// �����̹��� �迭
	UPROPERTY(BlueprintReadWrite, Category = "Data")
		int Arraysize = 5;													// ���� �̹��� ����

	// �ִϸ��̼� ����
	UPROPERTY(BlueprintReadWrite, Category = "Data")
		class UWidgetAnimation* WidgetAnim;						// ���� �ִϸ��̼�(�ǰݽ� HP�� ���Ÿ�)

	UPROPERTY()
		FTimerHandle TimerHandle;			// Ÿ�̸��ڵ�

	UFUNCTION()
		void AssignAnimations();			// �����ִϸ��̼� ã��
	UFUNCTION()
		void StartAnimations();			// �ִϸ��̼� ����
	UFUNCTION()
		void StopAnimations();			// �ִϸ��̼� ����
	UFUNCTION()
		void ReceiveDamage(float _damage);		// �������� ���� �� ����HP���ҿ� �ִϸ��̼� ����
	UFUNCTION()
		void GainHP(float _damage);				// HP�� ��� �Լ�
	UFUNCTION()
		void UseStamina(float _useValue);			// ���׹̳ʸ� ����ϴ� �Լ�
	UFUNCTION()
		void GainStamina(float _useValue);		// ���׹̳ʸ� ��� �Լ�
	UFUNCTION()
		void InvisiblePotion();							// ���� �̹��� pop�ϴ� �Լ�
	UFUNCTION()
		void AutoGainStamina();						// �ڵ������� ���׹̳ʰ� ä������ �Լ�
};
