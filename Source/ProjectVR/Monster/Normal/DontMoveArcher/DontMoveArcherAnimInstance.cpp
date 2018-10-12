// Fill out your copyright notice in the Description page of Project Settings.

#include "DontMoveArcherAnimInstance.h"
#include "DontMoveArcher.h"
#include "Monster/Normal/ArcherSword/Weapon/Bow/NMWeaponArrow.h"

void UDontMoveArcherAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);

	ADontMoveArcher* DontMoveArcher = Cast<ADontMoveArcher>(TryGetPawnOwner());

	if (DontMoveArcher && DontMoveArcher->IsValidLowLevelFast())
	{
		CurrentState = DontMoveArcher->CurrentState;
		CurrentAnimState = DontMoveArcher->CurrentAnimState;
		CurrentAttackState = DontMoveArcher->CurrentAttackState;
		CurrentArcherAttackState = DontMoveArcher->CurrentArcherAttackState;
		Pitch = DontMoveArcher->Pitch*-1.0f;
	}
}

void UDontMoveArcherAnimInstance::AnimNotify_ArrowSpawn(UAnimNotify * Notify)
{
	ADontMoveArcher* DontMoveArcher = Cast<ADontMoveArcher>(TryGetPawnOwner());

	if (DontMoveArcher)
	{
		DontMoveArcher->SpawnArrowMesh();
	}
}

void UDontMoveArcherAnimInstance::AnimNotify_ArrowDestroy(UAnimNotify * Notify)
{
	ADontMoveArcher* DontMoveArcher = Cast<ADontMoveArcher>(TryGetPawnOwner());

	if (DontMoveArcher)
	{
		FActorSpawnParameters SpawnActorOption;
		SpawnActorOption.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButDontSpawnIfColliding;

		DontMoveArcher->DeleteArrowMesh();

	}
}
