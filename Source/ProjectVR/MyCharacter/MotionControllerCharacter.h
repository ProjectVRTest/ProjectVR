// Fill out your copyright notice in the Description page of Project Settings.

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
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere)
		class UWidgetComponent* Widget;
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere)
		class UStereoLayerComponent* Stereo;
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere)
		class UBoxComponent* HeadBox;

	FTimerHandle DamageTimerHandle;
	FTimerHandle HitBloodyTimerHandle;
	FTimerHandle SetIdleTimerHandle;


	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "HP")
		float MaxHp;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "HP")
		float CurrentHp;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
		float MaxStamina;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
		float CurrentStamina;
	UPROPERTY()
		float RunningTime;
	UPROPERTY()
		bool bAllowBreathe;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
		float DashPower;
	
	UPROPERTY()
		bool InvincibleTimeOn;		// 무적시간인지 아닌지 판별

	UFUNCTION(BlueprintPure, Category = "Hit")
		bool PlayBloodyOverlay();
	UFUNCTION()
		void DisableBloody();
	UPROPERTY()
		bool bisHit = true;

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
	void DashUpStart();	
	UFUNCTION()
		void DashDownStart();
	UFUNCTION()
		void DashLeftStart();
	UFUNCTION()
		void DashRightStart();
	UFUNCTION()
		void DashEnd();

	void AttackPointSet();
		
	UFUNCTION()
		void SetAllowBreathe();
	UFUNCTION()
		virtual float TakeDamage(float Damage, struct FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser) override;		// 데미지 받기
	UFUNCTION()
		void DamageTimer();			// 무적시간 On 함수
	
	UFUNCTION()
		void OnHeadOverlap(UPrimitiveComponent * OverlappedComp, AActor * OtherActor, UPrimitiveComponent * OtherComp,
			int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult);		// 오버랩이벤트로 데미지를 전달하기위한 함수
};
