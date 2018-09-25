// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "State/DogState/DogState.h"
#include "Dog.generated.h"

UCLASS()
class PROJECTVR_API ADog : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ADog();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "HP")
		float MaxHP;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "HP")
		float CurrentHP;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "State")
		EDogState CurrentDogState;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "State")
		EDogAnimState CurrentDogAnimState;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "State")
		EDogBattleState CurrentDogBattleState;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "State")
		EDogJumpState CurrentDogJumpState;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "State")
		EDogCircleState CurrentDogCircleState;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "State")
		EDogAirState CurrentDogAirState;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "AI")
		class UPawnSensingComponent* PawnSensing;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "AI")
		class UBehaviorTree* BehaviorTree;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "AI")
		class AActor* Target;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "AI")
		class AActor* AttachActor;
	UFUNCTION()
		void OnSeePlayer(APawn * Pawn);
	class ADogAIController* AI;

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere)
		class UBoxComponent* DogAttackCollision;

	bool CurrentFalling;

	bool OnLandFlag;
	bool bpunchDetach;
	bool bIsDeath;
	bool bIsDetach;
	bool bOnLand;
	bool Landing;
	bool bIsBiting;					// 물었는지를 판단

	bool bIsLeftWander;
	bool bIsRightWander;


	// TEST
	bool once; // 서비스에서 배열에 딱 한번 들어가게 하기위해서 하는 것
	bool bAttack;   // 서비스에서 공격을 허락하는 부울값
	bool AttackWaite;	// 공격을 대기한다. true면 배회함

	bool bIsAttach;

	int point;
	float prelinear, preangular;

	UFUNCTION()
		void OnAttackCollisionOverlap(UPrimitiveComponent * OverlappedComp, AActor * OtherActor, UPrimitiveComponent * OtherComp,
			int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult);		// 오버랩이벤트로 데미지를 전달하기위한 함수

	UFUNCTION()
		virtual float TakeDamage(float Damage, struct FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser) override;		// 데미지 받기
};
