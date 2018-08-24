// Fill out your copyright notice in the Description page of Project Settings.

#include "AnimNotifyState_MBParryingArea.h"
#include "Headers/MiniBossAIHeader.h"
#include "Components/SkeletalMeshComponent.h"

void UAnimNotifyState_MBParryingArea::NotifyBegin(USkeletalMeshComponent * MeshComp, UAnimSequenceBase * Animation, float TotalDuration)
{
	Super::NotifyBegin(MeshComp, Animation, TotalDuration);

	AMiniBoss* MiniBoss = Cast<AMiniBoss>(MeshComp->GetOwner());

	if (MiniBoss)
	{
		MiniBoss->ParryingFlag = true;
	}
}

void UAnimNotifyState_MBParryingArea::NotifyTick(USkeletalMeshComponent * MeshComp, UAnimSequenceBase * Animation, float FrameDeltaTime)
{
	Super::NotifyTick(MeshComp, Animation, FrameDeltaTime);
}

void UAnimNotifyState_MBParryingArea::NotifyEnd(USkeletalMeshComponent * MeshComp, UAnimSequenceBase * Animation)
{
	Super::NotifyEnd(MeshComp, Animation);

	AMiniBoss* MiniBoss = Cast<AMiniBoss>(MeshComp->GetOwner());

	if (MiniBoss)
	{
		MiniBoss->ParryingFlag = false;
	}
}
