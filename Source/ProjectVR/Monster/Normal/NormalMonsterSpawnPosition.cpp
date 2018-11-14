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
	SPawnTime = 1.0f;
}

void ANormalMonsterSpawnPosition::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (NowSpawn)
	{
		SetSpawnTime(SPawnTime);
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

		Boss = Cast<ABoss>(GetOwner());
		
		if (Boss)
		{
			GLog->Log(FString::Printf(TEXT("부모가 보스")));
			SpawnActorOption.Owner = Boss;
		}
		else
		{
			GLog->Log(FString::Printf(TEXT("부모가 일반 스폰 포지션")));
			SpawnActorOption.Owner = this;
		}
		
		SpawnActorOption.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

		NormalMonster = GetWorld()->SpawnActor<ANormalMonster>(NormalMonster->StaticClass(), GetActorLocation(), FRotator::ZeroRotator, SpawnActorOption);

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
					NormalMonster->SetCurrentHP(MonsterHP);
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
					NormalMonster->SetCurrentHP(MonsterHP);
				}
			}
			break;
		}

		GetWorld()->GetTimerManager().SetTimer(FindTimer, this, &ANormalMonsterSpawnPosition::FindTarget, 1.0f, false);
		NowSpawn = false;
	}
}

void ANormalMonsterSpawnPosition::SetSpawnTime(float NewSpawnTime)
{
	SPawnTime = NewSpawnTime;
}
