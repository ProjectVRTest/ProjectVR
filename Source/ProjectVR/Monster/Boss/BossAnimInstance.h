// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "State/BossState/BossState.h"
#include "BossAnimInstance.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTVR_API UBossAnimInstance : public UAnimInstance
{
	GENERATED_BODY()
public:
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

	UFUNCTION(BlueprintCallable)
		virtual void NativeUpdateAnimation(float DeltaSeconds) override;

	UFUNCTION(BlueprintCallable, Category = "AI")
		void AnimNotify_LongDistanceAttackBallSpawn(UAnimNotify* Notify);
	UFUNCTION(BlueprintCallable, Category = "AI")
		void AnimNotify_Test(UAnimNotify* Notify);
	UFUNCTION(BlueprintCallable, Category = "AI")
		void AnimNotify_OrbCreate(UAnimNotify* Notify);
};
