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
		if (MiniBoss->Sword)
		{
			switch (MiniBoss->CurrentAttackState)
			{
			case EMiniBossAttackState::ShortAttack:

				switch (MiniBoss->CurrentShortAttackState)
				{
				case EMiniBossShortAttackState::RightUpLeftDownReady:
					MiniBoss->Sword->Damage = 15.0f;
					break;
				case EMiniBossShortAttackState::TwoHandWidthReady:
					MiniBoss->Sword->Damage = 10.0f;
					break;
				case EMiniBossShortAttackState::StabReady:
					MiniBoss->Sword->Damage = 15.0f;
					break;
				}
				break;
			case EMiniBossAttackState::WaveAttack:
				switch (MiniBoss->CurrentWaveAttackState)
				{
				case EMiniBossWaveAttackState::TwoHandWidthReady:
					MiniBoss->Sword->Damage = 20.0f;
					break;
				}
				break;
			case EMiniBossAttackState::ComboAttack:
				MiniBoss->Sword->Damage = 12.0f;
				break;
			case EMiniBossAttackState::BackAttack:
				MiniBoss->Sword->Damage = 10.0f;
				break;
			case EMiniBossAttackState::ParryingState:
				MiniBoss->Sword->Damage = 15.0f;
				break;
			}

			switch (MiniBoss->CurrentAnimState)
			{
			case EMiniBossAnimState::JumpAttack:
				MiniBoss->Sword->Damage = 25.0f;
				break;
			}

			MiniBoss->Sword->IsWeaponAttack = true;
		}
	}
}

void UAnimNotifyState_MBAttackArea::NotifyEnd(USkeletalMeshComponent * MeshComp, UAnimSequenceBase * Animation)
{
	Super::NotifyEnd(MeshComp, Animation);

	AMiniBoss* MiniBoss = Cast<AMiniBoss>(MeshComp->GetOwner());

	if (MiniBoss)
	{
		MiniBoss->Sword->IsWeaponAttack = false;
	}
}
