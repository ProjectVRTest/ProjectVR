// Fill out your copyright notice in the Description page of Project Settings.

#include "BTTask_BossUltimateAttackStart.h"
#include "Headers/BossAIHeader.h"
#include "Components/BoxComponent.h"
#include "Monster/Boss/Orb/Ultimate/BossRedOrb.h"
#include "Monster/Boss/Orb/Ultimate/BossBlueOrb.h"
#include "Monster/Boss/Orb/Ultimate/BossYellowOrb.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Particles/ParticleSystemComponent.h"
#include "Monster/Normal/NormalMonsterSpawnPosition.h"

void UBTTask_BossUltimateAttackStart::InitializeFromAsset(UBehaviorTree & Asset)
{
	Super::InitializeFromAsset(Asset);

	bNotifyTick = true;

}

EBTNodeResult::Type UBTTask_BossUltimateAttackStart::ExecuteTask(UBehaviorTreeComponent & OwnerComp, uint8 * NodeMemory)
{
	AI = Cast<ABossAIController>(OwnerComp.GetAIOwner());

	EndFlag = false;

	if (AI)
	{
		Boss = Cast<ABoss>(AI->GetPawn());
		if (Boss)
		{
			Boss->UltimateAuraEffectComponent->SetVisibility(true);
			OrbMaxCount = Boss->UltimateOrbColor.GetUltimateOrbMaxCount();
			SpawnNormalMaxCount = Boss->UltimateOrbColor.GetUltimateNormalMonsterSpawnMaxCount();

			//GetWorld()->GetTimerManager().SetTimer(UltimateEndTimer, this, &UBTTask_BossUltimateAttackStart::UltimateEnd, 20.0f, false);
			GetWorld()->GetTimerManager().SetTimer(UltimateSpawnLoopTimer, this, &UBTTask_BossUltimateAttackStart::UltimateLoop, 0.2f, true, 0.2f);
			EndFlag = true;
		}

	}
	return EBTNodeResult::InProgress;
}

void UBTTask_BossUltimateAttackStart::UltimateLoop()
{
	if (Boss)
	{
		FVector Origin;
		FVector Extent;
		float BoundRadius;

		FActorSpawnParameters SpawnActorOption;
		SpawnActorOption.Owner = Boss;
		SpawnActorOption.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButDontSpawnIfColliding;

		if (Boss->ManyOrbBound)
		{
			if (OrbMaxCount > 0)
			{
				UKismetSystemLibrary::GetComponentBounds(Boss->ManyOrbBound, Origin, Extent, BoundRadius);

				FVector RandomManyOrbCreatePoint = UKismetMathLibrary::RandomPointInBoundingBox(Origin, Extent);

				int RandomOrbColor = FMath::RandRange(1, 2);

				switch (RandomOrbColor)
				{
				case 1:
				{
					ABossRedOrb * RedOrb = GetWorld()->SpawnActor<ABossRedOrb>(RedOrb->StaticClass(), RandomManyOrbCreatePoint, Boss->GetActorRotation(), SpawnActorOption);

					Boss->UltimateOrbs.Add(RedOrb);
				}
				break;
				case 2:
				{
					ABossBlueOrb * BlueOrb = GetWorld()->SpawnActor<ABossBlueOrb>(BlueOrb->StaticClass(), RandomManyOrbCreatePoint, Boss->GetActorRotation(), SpawnActorOption);

					Boss->UltimateOrbs.Add(BlueOrb);
				}
				break;
				}
				OrbMaxCount--;
			}
			else
			{
				GetWorld()->GetTimerManager().ClearTimer(UltimateSpawnLoopTimer);
			}
		}

		if (Boss->MonsterSpawnBoound)
		{
			if (SpawnNormalMaxCount > 0)
			{
				UKismetSystemLibrary::GetComponentBounds(Boss->MonsterSpawnBoound, Origin, Extent, BoundRadius);

				FVector RandomNormalMonsterSpawnLocation = UKismetMathLibrary::RandomPointInBoundingBox(Origin, Extent);

				ANormalMonsterSpawnPosition* NormalSpawnPosition = GetWorld()->SpawnActor<ANormalMonsterSpawnPosition>(NormalSpawnPosition->StaticClass(), RandomNormalMonsterSpawnLocation, Boss->GetActorRotation(), SpawnActorOption);

				int RandomMonsterKind = FMath::RandRange(1, 2);

				if (RandomMonsterKind == 1)
				{
					NormalSpawnPosition->NormalMonsterkind = ENormalMonsterKind::MoveArcher;
				}
				else
				{
					NormalSpawnPosition->NormalMonsterkind = ENormalMonsterKind::SwordMan;
				}

				NormalSpawnPosition->NowSpawn = true;

				SpawnNormalMaxCount--;
			}
		}
	}
}

void UBTTask_BossUltimateAttackStart::TickTask(UBehaviorTreeComponent & OwnerComp, uint8 * NodeMemory, float DeltaSeconds)
{
	Super::TickTask(OwnerComp, NodeMemory, DeltaSeconds);

	if (Boss)
	{
		if (EndFlag)
		{
			Boss->CurrentUltimateAttackState = EBossUltimateAttackState::UltimateAttackStart;
			FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
		}		
	}
}
