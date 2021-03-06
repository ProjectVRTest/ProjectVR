﻿// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "State/NormalMonsterState/NormalMonsterState.h"
#include "NormalMonster.generated.h"

UCLASS()
class PROJECTVR_API ANormalMonster : public ACharacter
{
	GENERATED_BODY()

private:	
	UPROPERTY(VisibleAnywhere, Category = "Stat")
		float MaxHP; //최대 HP
	UPROPERTY(VisibleAnywhere, Category = "Stat")
		float CurrentHP; //현재 HP
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason)override;
public:	
	ANormalMonster();
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "State")
		ENormalMonsterState CurrentState;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "State")
		ENormalMonsterAnimState CurrentAnimState;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "IdleState")
		ENormalMonsterIdleState CurrentIdleState;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "State")
		ENormalMonsterAttackState CurrentAttackState;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "State")
		ENormalMonsterKind MonsterKind;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "State")
		ENormalMonsterStabAttackState CurrentStabAttackState;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "State")
		ENormalMonsterComboAttackState CurrentComboAttackState;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "State")
		ENormalMonsterArcherAttackState CurrentArcherAttackState;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "BodyMesh")
		class USkeletalMesh* ArcherSKMesh;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "BodyMesh")
		class USkeletalMesh* SwordSKMesh;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "AnimMontage")
		class UAnimMontage* NMAttackReverseMontage; //단타 공격 회복 애니메이션 몽타주
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "AI")
		class UPawnSensingComponent* PawnSensing;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "AI")
		class UBehaviorTree* SwordBehaviorTree;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "AI")
		class UBehaviorTree* MoveArcherBehaviorTree;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI")
		TArray<class AMyTargetPoint*> PatrolPoints;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "AI")
		class AActor* Target;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "AI")
		class ACameraLocation* TargetCamera;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Waepon")
		class UStaticMesh* QuiverMesh;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Waepon")
		class UStaticMeshComponent* NMArrowComponent;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Waepon")
		class USceneComponent* ArrowSpawnLocation;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Waepon")
		class UStaticMesh* ArrowMesh;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Weapon")
		class ANMWeaponSword* Sword;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Weapon")
		class ANMWeaponBow* Bow;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Weapon")
		class ANMWeaponQuiver* Quiver;
		
	UPROPERTY()
		FTimerHandle FresnelTimer;
	UPROPERTY()
		float FresnelValue;
	UPROPERTY()
		bool CanbeDamaged;
	UPROPERTY()
		int ArrowCount;
	UPROPERTY()
		bool DestroyFlag;
	UPROPERTY()
		bool MainMapSpawnFlag;
	UFUNCTION()
		void SpawnArrowMesh();
	UFUNCTION()
		void DeleteArrowMesh();
	UFUNCTION()
		void OnSeeCharacter(APawn *Pawn);
	UFUNCTION()
		void OnHearNoise(APawn * Pawn, const FVector & Location, float Volume);
	UFUNCTION()
		void Fresnel();
	UFUNCTION()
		virtual float TakeDamage(float Damage, struct FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser) override;
	UFUNCTION()
		void SetNormalMonsterKind(ENormalMonsterKind &NewMonsterKind);
	UFUNCTION()
		void SetEquipment();
	UFUNCTION()
		void SetTarget();
	UFUNCTION()
		void SetCurrentHP(float HP);
	UFUNCTION()
		void ChangeFormSword();
	UFUNCTION()
		void ChangeFormBow();
};
