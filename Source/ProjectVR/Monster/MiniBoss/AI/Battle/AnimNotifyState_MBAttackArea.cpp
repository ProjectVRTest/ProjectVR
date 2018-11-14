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
					MiniBoss->Sword->SetDamage(0.5f); //15
					break;
				case EMiniBossShortAttackState::TwoHandWidthReady:
					MiniBoss->Sword->SetDamage(0.5f);	 //10				
					break;
				case EMiniBossShortAttackState::StabReady:
					MiniBoss->Sword->SetDamage(0.5f); //15
					break;
				}
				break;
			case EMiniBossAttackState::WaveAttack:
				switch (MiniBoss->CurrentWaveAttackState)
				{
				case EMiniBossWaveAttackState::TwoHandWidthReady:
					MiniBoss->Sword->SetDamage(0.5f); //20
					break;
				}
				break;
			case EMiniBossAttackState::ComboAttack:
				MiniBoss->Sword->SetDamage(0.5f); //12
				break;
			case EMiniBossAttackState::BackAttack:
				MiniBoss->Sword->SetDamage(0.5f); //10
				break;
			case EMiniBossAttackState::ParryingState:
				MiniBoss->Sword->SetDamage(0.5f); //15
				break;
			}

			switch (MiniBoss->CurrentAnimState)
			{
			case EMiniBossAnimState::JumpAttack:
				MiniBoss->Sword->SetDamage(0.5f); //25
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
