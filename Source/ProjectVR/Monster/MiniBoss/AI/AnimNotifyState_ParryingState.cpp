// Fill out your copyright notice in the Description page of Project Settings.

#include "AnimNotifyState_ParryingState.h"

#include "Components/SkeletalMeshComponent.h"
#include "Monster/MiniBoss/MiniBoss.h"

void UAnimNotifyState_ParryingState::NotifyBegin(USkeletalMeshComponent * MeshComp, UAnimSequenceBase * Animation, float TotalDuration)
{
	Super::NotifyBegin(MeshComp, Animation, TotalDuration);

	AMiniBoss* MiniBoss = Cast<AMiniBoss>(MeshComp->GetOwner());

	if (MiniBoss)
	{
		MiniBoss->ParryingFlag = true;
	}
}

void UAnimNotifyState_ParryingState::NotifyEnd(USkeletalMeshComponent * MeshComp, UAnimSequenceBase * Animation)
{
	Super::NotifyEnd(MeshComp, Animation);

	AMiniBoss* MiniBoss = Cast<AMiniBoss>(MeshComp->GetOwner());

	if (MiniBoss)
	{
		MiniBoss->ParryingFlag = false;
	}
}
