// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "State/BossState/BossState.h"
#include "Boss.generated.h"

USTRUCT(BlueprintType)
struct PROJECTVR_API FUltimateOrbColors
{
	GENERATED_USTRUCT_BODY()
private:
	int32 UltimateNormalMonsterSpawnMaxCount;
	int32 UltimateOrbMaxCount;
public:
	int32 GetUltimateOrbMaxCount()
	{
		return UltimateOrbMaxCount;
	}
	int32 GetUltimateNormalMonsterSpawnMaxCount()
	{
		return UltimateNormalMonsterSpawnMaxCount;
	}

	void SetUltimateNormalMonsterSpawnMaxCount(int32 Count)
	{
		UltimateNormalMonsterSpawnMaxCount = Count;
	}

	void SetUltimateOrbMaxCount(int32 Count)
	{
		UltimateOrbMaxCount = Count;
	}
};

UCLASS()
class PROJECTVR_API ABoss : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ABoss();

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "State")
		EBossState CurrentState; //보스의 메인 상태 (대기,전투,그로기,죽음)
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "State")
		EBossBattleState CurrentBattleState; //보스의 전투 상태 (전투준비,애드 공격, 근접 공격, 원거리 공격, 전투 주시)
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "State")
		EBossBlinkAttackState CurrentBlinkAttackState;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "State")
		EBossLongAttackState CurrentLongAttackState;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "State")
		EBossCloseAttackState CurrentCloseAttackState;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "State")
		EBossParryingState CurrentParryingState;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "State")
		EBossBattleWatchState CurrentBattleWatchState;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "State")
		EBossConfrontationState CurrentConfrontationState;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "State")
		EBossUltimateAttackState CurrentUltimateAttackState;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "State")
		EBossWaveAttackState CurrentWaveAttackState;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "State")
		EBossUpDownAttackState CurrentUpDownAttackState;

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
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "OrbCreateLocation")
		class UBoxComponent* ManyOrbBound; //궁극기를 쓸때 오브들을 스폰시키는데 필요한 박스
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
		class UParticleSystemComponent* UltimateAuraEffectComponent;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "SwordWave")
		class UMonsterParryingManager* ParryingManager;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
		FTimerHandle BeginCharacterTimer;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Stat")
		float MaxHP;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Stat")
		float CurrentHP;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Effect")
		class UParticleSystem* BlinkSmoke;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Effect")
		class UParticleSystem* UltimateAura;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
		int32 OrbMaxCount; //기본 원거리 공격시 오브의 최대 갯수
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
		FUltimateOrbColors UltimateOrbColor;
	UPROPERTY()
		int32 CurrentNormalMonsterCount;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Parrying")
		int32 ParryingPointMaxCount; //HP에 따라서 확정된 패링카운트 갯수
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Parrying")
		int32 ParryingPointCount; //현재 기록된 패링카운트 갯수
	UPROPERTY(VisibleAnywhere, Category = "Parrying")
		TArray<FName>ParryingPoints; //패링포인트 소켓이름을 저장해둘 배열
	UPROPERTY(VisibleAnyWhere,Category = "TeleportPoint")
		FVector MapCenterLocation;
	UPROPERTY(VisibleAnyWhere,Category = "TeleportPoint")
		FVector UltimateStartLocation;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "TeleportPoint")
		TArray<class AActor*> TeleportPoints; //순간이동 지점을 저장해둘 배열
	UPROPERTY()
		TArray<class ABossOrb*> UltimateOrbs;
	UPROPERTY(VisibleAnyWhere, Category = "UltimateNMSPawnPosition")
		TArray<FVector> UltimateNormalMonsterSPawnPosition;
		

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
	UFUNCTION()
		void OnHearNoise(APawn * Pawn, const FVector & Location, float Volume);
	UFUNCTION()
		virtual float TakeDamage(float Damage, struct FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser) override;
	UFUNCTION()
		void FindCharacter();
};
