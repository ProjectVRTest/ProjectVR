// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "State/DontMoveArcherState/DontMoveArcherState.h"
#include "DontMoveArcher.generated.h"

UCLASS()
class PROJECTVR_API ADontMoveArcher : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ADontMoveArcher();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	void SpawnArrowMesh();

	void DeleteArrowMesh();

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "State")
		EDontMoveArcherState CurrentState;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "State")
		EDontMoveArcherAnimState CurrentAnimState;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "State")
		EDontMoveArcherAttackState CurrentAttackState;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "State")
		EDontMoveArcherArcherAttackState CurrentArcherAttackState;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "BodyMesh")
		class USkeletalMesh* SwordSKMesh;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "BodyMesh")
		class USkeletalMesh* ArcherSKMesh;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "AI")
		class UPawnSensingComponent* PawnSensing;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Waepon")
		class UStaticMeshComponent* QuiverComponent;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Waepon")
		class UStaticMesh* QuiverMesh;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Waepon")
		class UStaticMeshComponent* NMArrowComponent;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Waepon")
		class USceneComponent* ArrowSpawnLocation;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Waepon")
		class UStaticMesh* ArrowMesh;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Stat")
		float MaxHP; //최대 HP
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Stat")
		float CurrentHP; //현재 HP
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "AI")
		class AActor* Target;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI")
		class ACameraLocation* TargetCamera;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "AI")
		class UBehaviorTree* BehaviorTree;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Weapon")
		class ADontMoveArcherBow* Bow;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Pitch")
		float Pitch;
	bool AttackEndFlag = false;
	UFUNCTION()
		virtual float TakeDamage(float Damage, struct FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser) override;
	UFUNCTION()
	void OnSeeCharacter(APawn * Pawn);
};
