// Fill out your copyright notice in the Description page of Project Settings.

#include "AnimNotifyState_MBAttackArea.h"
#include "Headers/MiniBossAIHeader.h"
#include "Components/SkeletalMeshComponent.h"


void UAnimNotifyState_MBAttackArea::NotifyBegin(USkeletalMeshComponent * MeshComp, UAnimSequenceBase * Animation, float TotalDuration)
{
	Super::NotifyBegin(MeshComp, Animation, TotalDuration);

	AMiniBoss* MiniBoss = Cast<AMiniBoss>(MeshComp->GetOwner());

	if (MiniBoss)
	{
		MiniBoss->Sword->IsWeaponAttack = true;
		MiniBoss->Sword->IsParryingAttack = true;
	}
}

void UAnimNotifyState_MBAttackArea::NotifyTick(USkeletalMeshComponent * MeshComp, UAnimSequenceBase * Animation, float FrameDeltaTime)
{
	Super::NotifyTick(MeshComp, Animation, FrameDeltaTime);
}

void UAnimNotifyState_MBAttackArea::NotifyEnd(USkeletalMeshComponent * MeshComp, UAnimSequenceBase * Animation)
{
	Super::NotifyEnd(MeshComp, Animation);

	AMiniBoss* MiniBoss = Cast<AMiniBoss>(MeshComp->GetOwner());

	if (MiniBoss)
	{
		MiniBoss->Sword->IsWeaponAttack = false;
		MiniBoss->Sword->IsParryingAttack = false;
	}
}
