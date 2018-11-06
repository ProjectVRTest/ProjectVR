// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "HPStaminaBar_2.generated.h"

UCLASS()
class PROJECTVR_API AHPStaminaBar_2 : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AHPStaminaBar_2();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Scenes")
		class USceneComponent* Scene;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "HPBar")
		class UStaticMeshComponent* HPBackground;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "HPBar")
		class USceneComponent* HPScene;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "HPBar")
		class UStaticMeshComponent* HPBar;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "StaminaBar")
		class UStaticMeshComponent* StaminaBackground;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "StaminaBar")
		class USceneComponent* SteminaScene;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "StaminaBar")
		class UStaticMeshComponent* StaminaBar;

	UPROPERTY()
		class AMotionControllerCharacter* OwnerCharacter; //�÷��̾� ĳ����. ��ƽ���� ü�°� ���׹̳ʸ� �ҷ��� -> ���߿� �޸𸮻�뷮 ���ϰ���

	float ChangOfHP;				// HP ���� ��ȭ��
	float ChangOfStamina;		// ���׹̳� ���� ��ȭ��
	float Damage;					// ������

									// �÷��̾� ü������ ���� �ؾ���//
	float CurrentHP;
	float VirtualHP;
	float MaxHP;

	// �÷��̾� ���׹̳ʷ� ���� �ؾ���//
	float CurrentStamina;
	float VirtualStamina;
	float MaxStamina;
	float PreStamina;

	// �Լ�
	UFUNCTION()
		void GetDamage(float damage);		// �������� ���� �� ����HP���ҿ� �ִϸ��̼� ����
	UFUNCTION()
		void RecoveryHP(float recoveryHP);				// HP�� ��� �Լ�
	UFUNCTION()
		void UseStamina(float _useValue);			// ���׹̳ʸ� ����ϴ� �Լ�
	UFUNCTION()
		void RecoveryStamina(float _useValue);		// ���׹̳ʸ� ��� �Լ�
	UFUNCTION()
		void AutoGainStamina();						// �ڵ������� ���׹̳ʰ� ä������ �Լ�
	UPROPERTY()
		FTimerHandle AutoTimerHandle;			// Ÿ�̸��ڵ�
	
};
