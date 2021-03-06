﻿// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "MyCharacter/GripEnum.h"
#include "MotionControllerCharacter.generated.h"

UENUM(BlueprintType)
enum class EPlayerState :uint8
{
	Idle UMETA(Display = "Idle"),
	Walk UMETA(Display = "Walk"),
	Run UMETA(Display = "Run"),
	Attack UMETA(Display = "Attack"),
	Defense UMETA(Display = "Defense"),
	Hit UMETA(Display = "Hit"),
};

UCLASS()
class PROJECTVR_API AMotionControllerCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AMotionControllerCharacter();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "State")
		EPlayerState CurrentState; //캐릭터의 현재 상태
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "State")
		E_HandState GrabState;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera")
		class USpringArmComponent* SpringArm;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera")
		class UCameraComponent* Camera;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
		class ALeftHandMotionController* LeftHand;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
		class ARightHandMotionController* RightHand;
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere)
		class USceneComponent* Scene;
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category = "StateWidget")
		class UWidgetComponent* Widget;
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere)
		class UStereoLayerComponent* Stereo;
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere)
		class UBoxComponent* HeadBox;
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere)
		class ACameraLocation* CameraLocation;
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere)
		class UPawnNoiseEmitterComponent* NoiseEmitter;
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere)
		class UStaticMeshComponent* DamagedBlood;

	FTimerHandle DamageTimerHandle;
	FTimerHandle HitBloodyTimerHandle;
	FTimerHandle SetIdleTimerHandle;


	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "HP")
		float MaxHp;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "HP")
		float CurrentHp;
	UPROPERTY()
		float RunningTime;
	UPROPERTY()
		bool bAllowBreathe;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
		float DashPower;
	UPROPERTY()
		bool bDash;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Speed")
		float WalkSpeed;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Speed")
		float RunSpeed;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "StateInfo")
		float CurrentStamina;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "StateInfo")
		float MaxStamina;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "StateInfo")
		float AttackPoint;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "StateInfo")
		float DefencePoint;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "StateInfo")
		float DashPoint;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "StateInfo")
		float RecoveryPoint;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "StateInfo")
		bool bDeath;

	bool bIsUseStamina;

	FTimerHandle AutoTimerHandle;			// 타이머핸들
	FTimerHandle MoveMainHandle;				// 몇초후 메인화면으로 이동
	FTimerHandle DamagedHandle;			// 피격 화면 종료시간

	UPROPERTY()
		class UMaterialInstanceDynamic* DamagedMat_Inst;
	UFUNCTION()
		void FinishDamaged();
	UFUNCTION()
		bool UseStamina(float _stamina);
	UFUNCTION()
		void AutoStamina();			// 자동으로 스테미너 채워주는 함수

	bool bHasKey;		// 키를 가지고 있는지



	UPROPERTY()
		bool InvincibleTimeOn;		// 무적시간인지 아닌지 판별

	UFUNCTION(BlueprintPure, Category = "Hit")
		bool PlayBloodyOverlay();
	UFUNCTION()
		void DisableBloody();
	UPROPERTY()
		bool bisHit;
	UPROPERTY(VisibleAnywhere)
		float DamagedValue;
	float accTime;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AttackPoint")
		TArray<class AMyTargetPoint*> AttackPoints;

	UFUNCTION()
		void GrabLeftOn();
	UFUNCTION()
		void GrabLeftOff();
	UFUNCTION()
		void GrabRightOn();
	UFUNCTION()
		void GrabRightOff();
	UFUNCTION()
		void MoveForward(float Value);
	UFUNCTION()
		void MoveRight(float Value);

	UFUNCTION()
		void RunOn();
	UFUNCTION()
		void RunOff();
	UFUNCTION()
		void DashOn();
	UFUNCTION()
		void DashOff();

	UFUNCTION()
		void GameMenu();
	UPROPERTY()
		class AMenu* Menu;
	UFUNCTION()
		void SetAllowBreathe();
	UFUNCTION()
		virtual float TakeDamage(float Damage, struct FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser) override;		// 데미지 받기
	UFUNCTION()
		void DamageTimer();			// 무적시간 On 함수
	UFUNCTION()
		void MoveMainScene();
	UFUNCTION()
		void MainScene();

	UFUNCTION()
		void OnHeadOverlap(UPrimitiveComponent * OverlappedComp, AActor * OtherActor, UPrimitiveComponent * OtherComp,
			int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult);		// 오버랩이벤트로 데미지를 전달하기위한 함수

	UFUNCTION()
		void MakeNoiseEmitter(); //소리를 캐릭터 주위에 발생시켜주는 함수

	UPROPERTY()
		TArray<class ADog*> DogArray;
};
