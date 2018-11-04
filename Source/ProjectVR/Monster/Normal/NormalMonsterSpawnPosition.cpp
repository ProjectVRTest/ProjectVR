// Fill out your copyright notice in the Description page of Project Settings.

#include "NormalMonsterSpawnPosition.h"
#include "Monster/Normal/ArcherSword/NormalMonster.h"
#include "Monster/Normal/ArcherSword/NormalMonsterAIController.h"
#include "Kismet/GameplayStatics.h"
#include "Engine/World.h"
#include "TimerManager.h"
#include "UObject/ConstructorHelpers.h"
#include "Particles/ParticleSystem.h"
#include "Monster/Boss/Boss.h"

ANormalMonsterSpawnPosition::ANormalMonsterSpawnPosition()
{
	PrimaryActorTick.bCanEverTick = true;

	static ConstructorHelpers::FObjectFinder<UParticleSystem>PT_SpawnEffect(TEXT("ParticleSystem'/Game/Assets/Effect/ES_Skill/PT_BossBlinkSmoke.PT_BossBlinkSmoke'"));
	if (PT_SpawnEffect.Succeeded())
	{
		SpawnEffect = PT_SpawnEffect.Object;
	}

	NowSpawn = false;
}

void ANormalMonsterSpawnPosition::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (NowSpawn)
	{
		MonsterSpawn();
	}
}

void ANormalMonsterSpawnPosition::FindTarget()
{
	NormalMonster->SetTarget();
	Destroy();
}

void ANormalMonsterSpawnPosition::MonsterSpawn()
{
	ENormalMonsterKind MonsterKind;

	NormalMonsterAI = GetWorld()->SpawnActor<ANormalMonsterAIController>(NormalMonsterAI->StaticClass(), GetActorLocation(), GetActorRotation());

	if (NormalMonsterAI)
	{
		FActorSpawnParameters SpawnActorOption;

		ABoss* Boss = Cast<ABoss>(GetOwner());
		
		if (Boss)
		{
			SpawnActorOption.Owner = Boss;
		}
		else
		{
			SpawnActorOption.Owner = this;
		}
		
		SpawnActorOption.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

		NormalMonster = GetWorld()->SpawnActor<ANormalMonster>(NormalMonster->StaticClass(), GetActorLocation(), GetActorRotation(), SpawnActorOption);

		UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), SpawnEffect, NormalMonster->GetActorLocation());


		switch (NormalMonsterkind)
		{
		case ENormalMonsterKind::SwordMan:
			if (NormalMonster)
			{
				MonsterKind = ENormalMonsterKind::SwordMan;
				NormalMonster->SetNormalMonsterKind(MonsterKind);
				if (NormalMonsterAI)
				{
					NormalMonsterAI->Possess(NormalMonster);
					NormalMonster->SetCurrentHP(5.0f);
				}
			}
			break;
		case ENormalMonsterKind::MoveArcher:
			if (NormalMonster)
			{
				MonsterKind = ENormalMonsterKind::MoveArcher;
				NormalMonster->SetNormalMonsterKind(MonsterKind);
				if (NormalMonsterAI)
				{
					NormalMonsterAI->Possess(NormalMonster);
					NormalMonster->SetCurrentHP(5.0f);
				}
			}
			break;
		}

		GetWorld()->GetTimerManager().SetTimer(FindTimer, this, &ANormalMonsterSpawnPosition::FindTarget, 1.0f, false);
		NowSpawn = false;
	}
}
