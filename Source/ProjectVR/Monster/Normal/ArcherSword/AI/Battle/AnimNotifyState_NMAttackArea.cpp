// Fill out your copyright notice in the Description page of Project Settings.

#include "AnimNotifyState_NMAttackArea.h"
#include "Headers/NormalMonsterAIHeader.h"
#include "Components/SkeletalMeshComponent.h"

void UAnimNotifyState_NMAttackArea::NotifyBegin(USkeletalMeshComponent * MeshComp, UAnimSequenceBase * Animation, float TotalDuration)
{
	Super::NotifyBegin(MeshComp, Animation, TotalDuration);

	ANormalMonster* NormalMonster = Cast<ANormalMonster>(MeshComp->GetOwner());

	if (NormalMonster)
	{
		NormalMonster->Sword->IsWeaponAttack = true;
	}
}

void UAnimNotifyState_NMAttackArea::NotifyTick(USkeletalMeshComponent * MeshComp, UAnimSequenceBase * Animation, float FrameDeltaTime)
{
	Super::NotifyTick(MeshComp, Animation, FrameDeltaTime);
}

void UAnimNotifyState_NMAttackArea::NotifyEnd(USkeletalMeshComponent * MeshComp, UAnimSequenceBase * Animation)
{
	Super::NotifyEnd(MeshComp, Animation);

	ANormalMonster* NormalMonster = Cast<ANormalMonster>(MeshComp->GetOwner());

	if (NormalMonster)
	{
		NormalMonster->Sword->IsWeaponAttack = false;
	}
}




