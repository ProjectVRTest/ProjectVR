// Fill out your copyright notice in the Description page of Project Settings.

#include "AnimNotifyState_BossParryingArea.h"
#include "Headers/BossAIHeader.h"

void UAnimNotifyState_BossParryingArea::NotifyBegin(USkeletalMeshComponent * MeshComp, UAnimSequenceBase * Animation, float TotalDuration)
{
	Super::NotifyBegin(MeshComp, Animation, TotalDuration);

}

void UAnimNotifyState_BossParryingArea::NotifyEnd(USkeletalMeshComponent * MeshComp, UAnimSequenceBase * Animation)
{
	Super::NotifyEnd(MeshComp, Animation);

}
