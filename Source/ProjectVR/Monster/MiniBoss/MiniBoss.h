// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "State/MiniBossState/MiniBossState.h"
#include "MiniBoss.generated.h"

UCLASS()
class PROJECTVR_API AMiniBoss : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AMiniBoss();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "State")
		EMiniBossState CurrentState;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "State")
		EMiniBossAnimState CurrentAnimState;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "State")
		EMiniBossJumpState CurrentJumpState;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "State")
		EMiniBossAttackState CurrentAttackState;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "State")
		EMiniBossShortAttackState CurrentShortAttackState;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "State")
		EMiniBossWaveAttackState CurrentWaveAttackState;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "State")
		EMiniBossComboAttackState CurrentComboAttackState;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "State")
		EMiniBossBackAttackState CurrentBackAttackState;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "State")
		EMiniBossDashState CurrentDashState;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "State")
		EMiniBossParryingState CurrentParryingState;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "AI")
		class UPawnSensingComponent* PawnSensing;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "AI")
		class UBehaviorTree* BehaviorTree;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI")
		class AActor* Target; 
	
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "AnimMontage")
		class UAnimMontage* AttackReverseMontage; //피격 애니메이션
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Jump")
		bool CurrentFalling; //캐릭터가 현재 추락하고 있는지 확인해줄 변수 
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Parrying")
		bool ParryingFlag; //패링이 가능한 구간에 true로 바뀌고 구간이 끝나면 false로 바뀌는 변수
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Parrying")
		bool IsParrying; //현재 패링중인지 아닌지
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Stat")
		float MaxHP; //최대 HP
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Stat")
		float CurrentHP; //현재 HP
	class UMaterialInterface* OpacityMaterials;
	class UMaterialInterface* DefaultMaterials;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Effect")
		class UParticleSystem* AfterImageStartEffect;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Effect")
		class UParticleSystem* AfterImageEndEffect;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Weapon")
		class AMiniBossWeapon* Sword;
	float Yaw; //캐릭터가 좌우로 움직일때 판단해줄 값
	bool WalkStopFlag; 
	bool IsAttack; //공격중인지 아닌지 판단
	bool AttackCompleteFlag; //공격이 완료 됏는지 판단
	bool StabFlag; //찌르기 애니메이션의 재생을 판단해줄 변수로 true이면 찌르기 노콤보 애니메이션이 재생되고, false이면 찌르기 콤보 애니메이션이 재생된다.
	bool TwoHandWidthFlag; //가로베기 애니메이션의 재생을 판단해줄 변수로 true이면 가로베기 노콤보 애니메이션이 재생되고, false이면 가로베기 콤보 애니메이션이 재생된다.


	UFUNCTION()
		void OnSeeCharacter(APawn *Pawn); //캐릭터를 감지하면 실행해줄 함수
	UFUNCTION()
		virtual float TakeDamage(float Damage, struct FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser) override;
};
