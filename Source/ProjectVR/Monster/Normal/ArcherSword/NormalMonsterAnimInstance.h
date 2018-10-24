// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "State/NormalMonsterState/NormalMonsterState.h"
#include "NormalMonsterAnimInstance.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTVR_API UNormalMonsterAnimInstance : public UAnimInstance
{
	GENERATED_BODY()
public:
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

	UFUNCTION(BlueprintCallable)
		virtual void NativeUpdateAnimation(float DeltaSeconds) override;	
	UFUNCTION(BlueprintCallable, Category = "Bow")
		void AnimNotify_ArrowSpawn(UAnimNotify* Notify);
	UFUNCTION(BlueprintCallable, Category = "Bow")
		void AnimNotify_ArrowDestroy(UAnimNotify* Notify);
};
