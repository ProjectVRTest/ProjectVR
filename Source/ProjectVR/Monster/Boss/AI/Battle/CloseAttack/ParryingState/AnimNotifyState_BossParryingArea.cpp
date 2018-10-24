// Fill out your copyright notice in the Description page of Project Settings.

#include "AnimNotifyState_BossParryingArea.h"
#include "Headers/BossAIHeader.h"
#include "Components/SkeletalMeshComponent.h"
#include "Monster/Boss/Weapon/BossWeapon.h"

void UAnimNotifyState_BossParryingArea::NotifyBegin(USkeletalMeshComponent * MeshComp, UAnimSequenceBase * Animation, float TotalDuration)
{
	Super::NotifyBegin(MeshComp, Animation, TotalDuration);

	ABoss* Boss = Cast<ABoss>(MeshComp->GetOwner());

	if (Boss)
	{
		Boss->Sickle->IsParryingAttack = true;
	}
}

void UAnimNotifyState_BossParryingArea::NotifyEnd(USkeletalMeshComponent * MeshComp, UAnimSequenceBase * Animation)
{
	Super::NotifyEnd(MeshComp, Animation);

	ABoss* Boss = Cast<ABoss>(MeshComp->GetOwner());

	if (Boss)
	{
		Boss->Sickle->IsParryingAttack = false;
	}
}
