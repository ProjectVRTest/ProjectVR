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
		switch (NormalMonster->CurrentState)
		{
		case ENormalMonsterState::Chase:
			switch (NormalMonster->CurrentAnimState)
			{
			case ENormalMonsterAnimState::RageAttack:
				NormalMonster->Sword->SetDamage(0.5f);				//20
				break;
			}
			break;
		case ENormalMonsterState::Battle:
			switch (NormalMonster->CurrentAttackState)
			{
			case ENormalMonsterAttackState::StabAttack:
				if (NormalMonster->CurrentStabAttackState == ENormalMonsterStabAttackState::StabAttackReady)
				{
					NormalMonster->Sword->SetDamage(0.5f); //15
				}
				else if(NormalMonster->CurrentStabAttackState == ENormalMonsterStabAttackState::ClipplingAttackReady)
				{
					NormalMonster->Sword->SetDamage(0.5f);//20
				}
				break;
			case ENormalMonsterAttackState::ComboAttack:
				NormalMonster->Sword->SetDamage(0.5f); //10
				break;
			}
			break;
		}

		if (NormalMonster->Sword)
		{
			NormalMonster->Sword->IsWeaponAttack = true;
		}		
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
		if (NormalMonster->Sword)
		{
			NormalMonster->Sword->IsWeaponAttack = false;
		}		
	}
}




