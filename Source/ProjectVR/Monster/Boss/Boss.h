// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "State/BossState/BossState.h"
#include "Boss.generated.h"

UCLASS()
class PROJECTVR_API ABoss : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ABoss();

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "State")
		EBossState CurrentState;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "State")
		EBossBattleState CurrentBattleState;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "State")
		EBossBlinkAttackState CurrentBlinkAttackState;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "State")
		EBossLongAttackState CurrentLongAttackState;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "State")
		EBossCloseAttackState CurrentCloseAttackState;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "State")
		EBossParryingState CurrentParryingState;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "AI")
		class UPawnSensingComponent* PawnSensing;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "AI")
		class UBehaviorTree* BehaviorTree;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Weapon")
		class ABossWeapon* Sickle;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "AI")
		class AActor* Target;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "AI")
		class ACameraLocation* TargetCamera;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Weapon")
		class USceneComponent* SwordWaveSpawn;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Weapon")
		class USceneComponent* WaveSpawn;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Materials")
		class UMaterialInterface* OpacityMaterials;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Materials")
		class UMaterialInterface* DefaultBodyMaterials;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Materials")
		class UMaterialInterface* DefaultClothMaterials;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "AttackBallCrashLocation")
		class USceneComponent* LeftCrashLocation;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "AttackBallCrashLocation")
		class USceneComponent* RightCrashLocation;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "OrbCreateLocation")
		class USceneComponent* OrbCreateLocation;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Effect")
		class UParticleSystem* BlinkSmoke;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
		int OrbMaxCount;

	UPROPERTY()
		TArray<FName>ParryingPoints; //패링포인트 소켓이름을 저장해둘 배열	

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	UFUNCTION()
		void ParryingPointInit(); //패링포인트 배열에 패링포인트소켓이름을 저장해주는 함수
	UFUNCTION()
		void ParryingPointSet(); //패링포인트를 중간보스의 HP상태에 따라 스폰해주는 함수
	UFUNCTION()
		void ParryingPointValueSet(int ParryingCount); //HP가 50%이하인경우 지정한 패링포인트 수만큼 랜덤하게 스폰해주는 함수
	UFUNCTION()
		void OnSeeCharacter(APawn * Pawn);	
};
