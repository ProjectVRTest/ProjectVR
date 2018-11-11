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
		if (Boss->Sickle)
		{

			switch (Boss->CurrentBattleState)
			{
			case EBossBattleState::CloseAttack:
				switch (Boss->CurrentCloseAttackState)
				{
				case EBossCloseAttackState::RightUpLeftDownAttack:
					Boss->Sickle->SetDamage(18.0f);
					break;
				case EBossCloseAttackState::ComboAttack:
					Boss->Sickle->SetDamage(15.0f);
					break;
				case EBossCloseAttackState::UpDownAttack:
					Boss->Sickle->SetDamage(20.0f);
					break;
				}

				switch (Boss->CurrentParryingState)
				{
				case EBossParryingState::CounterAttack:
					Boss->Sickle->SetDamage(20.0f);
					break;
				}
				break;
			case EBossBattleState::AddAttack:
				switch (Boss->CurrentBlinkAttackState)
				{
				case EBossBlinkAttackState::BlinkAttack:
					Boss->Sickle->SetDamage(20.0f);
					break;
				}
				break;
			}
		}
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
