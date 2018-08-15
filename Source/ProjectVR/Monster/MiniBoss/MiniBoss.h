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
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "AI")
		class UPawnSensingComponent* PawnSensing;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "AI")
		class UBehaviorTree* BehaviorTree;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI")
		class AActor* Target; 
	
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "AnimMontage")
		class UAnimMontage* ReactionMontage; //피격 애니메이션
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Jump")
		bool CurrentFalling; //캐릭터가 현재 추락하고 있는지 확인해줄 변수
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Jump")
		bool JumpEndFlag; //점프가 끝났음을 알려주는 변수
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Jump")
		bool JumpRunCheckFlag; //점프 or 달리기를 다시 설정할수 있게 해주는 변수 
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
	float Yaw;
	bool WalkStopFlag;
	bool BackAttack;

	UFUNCTION()
		void OnSeeCharacter(APawn *Pawn); //캐릭터를 감지하면 실행해줄 함수
	UFUNCTION()
		virtual float TakeDamage(float Damage, struct FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser) override;
};
