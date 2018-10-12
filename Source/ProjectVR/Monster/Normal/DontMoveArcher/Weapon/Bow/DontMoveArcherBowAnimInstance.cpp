// Fill out your copyright notice in the Description page of Project Settings.

#include "DontMoveArcherBowAnimInstance.h"
#include "Monster/Normal/DontMoveArcher/DontMoveArcher.h"
#include "Monster/Normal/DontMoveArcher/Weapon/Bow/DontMoveArcherBow.h"

void UDontMoveArcherBowAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);

	ADontMoveArcherBow* Bow = Cast<ADontMoveArcherBow>(GetOwningActor());

	if (Bow)
	{
		ADontMoveArcher* DontMoveArcher = Cast<ADontMoveArcher>(Bow->GetAttachParentActor());

		if (DontMoveArcher)
		{
			CurrentAttackState = DontMoveArcher->CurrentAttackState;
			CurrentArcherAttackState = DontMoveArcher->CurrentArcherAttackState;
		}
	}
}
