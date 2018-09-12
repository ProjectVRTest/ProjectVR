// Fill out your copyright notice in the Description page of Project Settings.

#include "NormalMonsterAnimInstance.h"
#include "NormalMonster.h"

void UNormalMonsterAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);

	ANormalMonster* NormalMonster = Cast<ANormalMonster>(TryGetPawnOwner());

	if (NormalMonster && NormalMonster->IsValidLowLevelFast())
	{
		CurrentState = NormalMonster->CurrentState;
		CurrentAnimState = NormalMonster->CurrentAnimState;
		CurrentIdleState = NormalMonster->CurrentIdleState;
		CurrentAttackState = NormalMonster->CurrentAttackState;
		CurrentStabAttackState = NormalMonster->CurrentStabAttackState;
		CurrentComboAttackState = NormalMonster->CurrentComboAttackState;
		CurrentArcherAttackState = NormalMonster->CurrentArcherAttackState;
	}
}

void UNormalMonsterAnimInstance::AnimNotify_AttackEnd(UAnimNotify * Notify)
{
	ANormalMonster* NormalMonster = Cast<ANormalMonster>(TryGetPawnOwner());

	if (NormalMonster)
	{
		NormalMonster->NMAttackEndFlag = true;
	}
}
