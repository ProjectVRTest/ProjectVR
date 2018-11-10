// Fill out your copyright notice in the Description page of Project Settings.

#include "NormalMonsterAnimInstance.h"
#include "NormalMonster.h"
#include "Monster/Normal/ArcherSword/Weapon/Bow/NMWeaponArrow.h"
#include "Monster/SwordWaveTarget.h"
#include "MyCharacter/CameraLocation.h"
#include "Monster/Normal/ArcherSword/Weapon/Sword/NMWeaponSword.h"

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

void UNormalMonsterAnimInstance::AnimNotify_ArrowSpawn(UAnimNotify * Notify)
{
	ANormalMonster* NormalMonster = Cast<ANormalMonster>(TryGetPawnOwner());

	if (NormalMonster)
	{
		NormalMonster->SpawnArrowMesh();
	}
}

void UNormalMonsterAnimInstance::AnimNotify_ArrowDestroy(UAnimNotify * Notify)
{
	ANormalMonster* NormalMonster = Cast<ANormalMonster>(TryGetPawnOwner());

	if (NormalMonster)
	{
		FActorSpawnParameters SpawnActorOption;
		SpawnActorOption.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButDontSpawnIfColliding;

		NormalMonster->DeleteArrowMesh();

		FVector LockonTargetLocation = NormalMonster->TargetCamera->GetActorLocation();

		ASwordWaveTarget* SwordWaveTarget = GetWorld()->SpawnActor<ASwordWaveTarget>(SwordWaveTarget->StaticClass(), LockonTargetLocation, FRotator::ZeroRotator);
		
		if (SwordWaveTarget)
		{
			ANMWeaponArrow* NMArrow = GetWorld()->SpawnActor<ANMWeaponArrow>(NMArrow->StaticClass(), NormalMonster->ArrowSpawnLocation->GetComponentLocation(), NormalMonster->GetActorRotation(), SpawnActorOption);

			NormalMonster->ArrowCount--;

			if (NMArrow)
			{
				NMArrow->Homing(SwordWaveTarget);
			}

			if (NormalMonster->ArrowCount == 0)
			{
				NormalMonster->ChangeFormSword();
			}
		}		
	}
}
