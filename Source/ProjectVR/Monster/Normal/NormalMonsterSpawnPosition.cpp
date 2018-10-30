// Fill out your copyright notice in the Description page of Project Settings.

#include "NormalMonsterSpawnPosition.h"
#include "Monster/Normal/ArcherSword/NormalMonster.h"
#include "Monster/Normal/ArcherSword/NormalMonsterAIController.h"
#include "Kismet/GameplayStatics.h"
#include "Engine/World.h"
#include "TimerManager.h"

ANormalMonsterSpawnPosition::ANormalMonsterSpawnPosition()
{

}

void ANormalMonsterSpawnPosition::FindTarget()
{
	NormalMonster->SetTarget();
}

void ANormalMonsterSpawnPosition::BeginPlay()
{
	Super::BeginPlay();

	ENormalMonsterKind MonsterKind;

	NormalMonsterAI = GetWorld()->SpawnActor<ANormalMonsterAIController>(NormalMonsterAI->StaticClass(), GetActorLocation(), GetActorRotation());

	switch (NormalMonsterkind)
	{
	case ENormalMonsterKind::SwordMan:
		NormalMonster = GetWorld()->SpawnActor<ANormalMonster>(NormalMonster->StaticClass(), GetActorLocation(), GetActorRotation());
		
		if (NormalMonster)
		{
			MonsterKind = ENormalMonsterKind::SwordMan;
			NormalMonster->SetNormalMonsterKind(MonsterKind);
			if (NormalMonsterAI)
			{
				NormalMonsterAI->Possess(NormalMonster);
				
			}
		}		
		break;
	case ENormalMonsterKind::MoveArcher:
		NormalMonster = GetWorld()->SpawnActor<ANormalMonster>(NormalMonster->StaticClass(), GetActorLocation(), GetActorRotation());
		
		if (NormalMonster)
		{
			MonsterKind = ENormalMonsterKind::MoveArcher;
			NormalMonster->SetNormalMonsterKind(MonsterKind);
			if (NormalMonsterAI)
			{
				NormalMonsterAI->Possess(NormalMonster);				
			}
		}
		break;
	}	

	GetWorld()->GetTimerManager().SetTimer(FindTimer, this, &ANormalMonsterSpawnPosition::FindTarget, 1.0f, false);
}


