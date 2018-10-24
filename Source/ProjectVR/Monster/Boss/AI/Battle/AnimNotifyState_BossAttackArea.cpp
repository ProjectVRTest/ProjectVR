// Fill out your copyright notice in the Description page of Project Settings.

#include "AnimNotifyState_BossAttackArea.h"
#include "Headers/BossAIHeader.h"
#include "Components/SkeletalMeshComponent.h"
#include "Monster/Boss/Weapon/BossWeapon.h"

void UAnimNotifyState_BossAttackArea::NotifyBegin(USkeletalMeshComponent * MeshComp, UAnimSequenceBase * Animation, float TotalDuration)
{
	Super::NotifyBegin(MeshComp, Animation, TotalDuration);

	ABoss* Boss = Cast<ABoss>(MeshComp->GetOwner());

	if (Boss)
	{
		Boss->Sickle->IsWeaponAttack = true;
	}
}

void UAnimNotifyState_BossAttackArea::NotifyEnd(USkeletalMeshComponent * MeshComp, UAnimSequenceBase * Animation)
{
	Super::NotifyEnd(MeshComp, Animation);
		
	ABoss* Boss = Cast<ABoss>(MeshComp->GetOwner());

	if (Boss)
	{
		Boss->Sickle->IsWeaponAttack = false;
	}
}
