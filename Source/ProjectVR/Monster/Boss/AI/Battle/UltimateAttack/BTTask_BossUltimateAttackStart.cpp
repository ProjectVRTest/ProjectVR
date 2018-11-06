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
			
			GetWorld()->GetTimerManager().SetTimer(UltimateEndTimer, this, &UBTTask_BossUltimateAttackStart::UltimateEnd, 20.0f, false);
			GetWorld()->GetTimerManager().SetTimer(UltimateSpawnLoopTimer, this, &UBTTask_BossUltimateAttackStart::UltimateLoop, 0.2f, true, 0.2f);
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
		}

		if (Boss->MonsterSpawnBoound)
		{
			if (SpawnNormalMaxCount > 0)
			{
				GLog->Log(FString::Printf(TEXT("일반몬스터 소환")));
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
		CurrentNormalMonsterCount = Boss->CurrentNormalMonsterCount;

		if (EndFlag || CurrentNormalMonsterCount <=0)
		{
			if (CurrentNormalMonsterCount <= 0)
			{
				UltimateEnd(); //동시에 끝날 경우를 판정해줘야할 조건이 필요함
			}

			GLog->Log(FString::Printf(TEXT("궁극기 시전 시간 끝남")));
			Boss->CurrentNormalMonsterCount = 3;
			Boss->UltimateAuraEffectComponent->SetVisibility(false);
			Boss->GetCharacterMovement()->GravityScale = 1.0f;
			Boss->CurrentUltimateAttackState = EBossUltimateAttackState::Idle;
			Boss->CurrentBattleState = EBossBattleState::AttackReady;
			FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
		}

	}
}

void UBTTask_BossUltimateAttackStart::UltimateEnd()
{
	if (Boss->CurrentBattleState == EBossBattleState::UltimateAttack)
	{
		EndFlag = true; //현재 상태를 탈출시켜 주기 위해 EndFlag를 켜준다.

		if (Boss->UltimateOrbs.Num() > 0)
		{
			for (auto Orb : Boss->UltimateOrbs)
			{
				if (Orb)
				{
					Orb->Destroy(); //남아있는 오브가 있으면 삭제 시킨다.
				}
			}

			Boss->UltimateOrbs.Empty(); //궁극기 오브를 담아둔 배열을 깨끗하게 정리한다.
		}		
		GLog->Log(FString::Printf(TEXT("남아 있는 오브 수 %d"), Boss->UltimateOrbs.Num()));
	}
}

