// Fill out your copyright notice in the Description page of Project Settings.

#include "NMWeaponBowAnimInstance.h"
#include "Monster/Normal/NormalMonster.h"
#include "Monster/Normal/Weapon/Bow/NMWeaponBow.h"
void UNMWeaponBowAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);
	
	ANMWeaponBow* Bow = Cast<ANMWeaponBow>(GetOwningActor());

	if (Bow)
	{
		ANormalMonster* NormalMonster = Cast<ANormalMonster>(Bow->GetAttachParentActor());
		if (NormalMonster)
		{
			CurrentAttackState = NormalMonster->CurrentAttackState;
			CurrentArcherAttackState = NormalMonster->CurrentArcherAttackState;
		}		
	}
}