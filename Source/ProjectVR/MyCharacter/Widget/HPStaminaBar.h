// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "HPStaminaBar.generated.h"

UCLASS()
class PROJECTVR_API AHPStaminaBar : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AHPStaminaBar();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Scenes")
		class USceneComponent* Scene;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "HPBar")
		class USceneComponent* HPScene;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "HPBar")
		class UStaticMeshComponent* HPBar;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "StaminaBar")
		class USceneComponent* SteminaScene;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "StaminaBar")
		class UStaticMeshComponent* StaminaBar;

	UPROPERTY(VisibleAnywhere, Category = "Size")
		float HPBarXSize;
	UPROPERTY(VisibleAnywhere, Category = "Size")
		float HPBarZSize;
	UPROPERTY(VisibleAnywhere, Category = "Size")
		float SteminaBarXSize;
	UPROPERTY(VisibleAnywhere, Category = "Size")
		float SteminaBarZSize;

	UPROPERTY()
		class AMotionControllerCharacter* OwnerCharacter; //플레이어 캐릭터. 매틱마다 체력과 스테미너를 불러옴 -> 나중에 메모리사용량 줄일거임

	float ChangOfHP;				// HP 증감 변화량
	float ChangOfStamina;		// 스테미너 증감 변화량
	float Damage;					// 데미지

	// 플레이어 체력으로 연결 해야함//
	float CurrentHP;
	float VirtualHP;
	float MaxHP;

	// 플레이어 스테미너로 연결 해야함//
	float CurrentStamina;
	float VirtualStamina;
	float MaxStamina;
	float PreStamina;

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
};
