// Fill out your copyright notice in the Description page of Project Settings.

#include "BossAnimInstance.h"
#include "Monster/Boss/Boss.h"
#include "Monster/Boss/AI/AddAttack/BossAddAttackBall.h"
#include "MyCharacter/MotionControllerCharacter.h"
#include "MyCharacter/CameraLocation.h"
#include "Monster/Boss/AI/AddAttack/BossAddAttackBall.h"
#include "Components/SphereComponent.h"
#include "BossWaveTarget.h"
#include "Orb/DefaultOrb/BossOrb.h"
#include "Weapon/BossWaveAttackWave.h"

void UBossAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);

	ABoss* Boss = Cast<ABoss>(TryGetPawnOwner());

	if (Boss && Boss->IsValidLowLevelFast())
	{
		CurrentState = Boss->CurrentState;
		CurrentBattleState = Boss->CurrentBattleState;
		CurrentBlinkAttackState = Boss->CurrentBlinkAttackState;
		CurrentLongAttackState = Boss->CurrentLongAttackState;
		CurrentCloseAttackState = Boss->CurrentCloseAttackState;
		CurrentParryingState = Boss->CurrentParryingState;
		CurrentBattleWatchState = Boss->CurrentBattleWatchState;
		CurrentConfrontationState = Boss->CurrentConfrontationState;
		CurrentUltimateAttackState = Boss->CurrentUltimateAttackState;
		CurrentWaveAttackState = Boss->CurrentWaveAttackState;
		CurrentUpDownAttackState = Boss->CurrentUpDownAttackState;
	}
}

void UBossAnimInstance::AnimNotify_LongDistanceAttackBallSpawn(UAnimNotify * Notify)
{
	ABoss* Boss = Cast<ABoss>(TryGetPawnOwner());
	GLog->Log(FString::Printf(TEXT("원거리 공격 생성")));
	if (Boss)
	{
		int LeftRightValue;
		FVector LockonTargetLocation;
		FVector Location;
		ABossWaveTarget* BossWaveTarget;
		ABossAddAttackBall* AddAttackBall;

		FActorSpawnParameters SpawnActorOption;
		SpawnActorOption.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButDontSpawnIfColliding;
		LeftRightValue = FMath::RandRange(0, 1);

		if (LeftRightValue == 0)
		{
			LockonTargetLocation = Boss->LeftCrashLocation->GetComponentLocation();
		}
		else
		{
			LockonTargetLocation = Boss->RightCrashLocation->GetComponentLocation();
		}		
		
		BossWaveTarget = GetWorld()->SpawnActor<ABossWaveTarget>(BossWaveTarget->StaticClass(), LockonTargetLocation, FRotator::ZeroRotator);

		AddAttackBall = GetWorld()->SpawnActor<ABossAddAttackBall>(AddAttackBall->StaticClass(), Boss->SwordWaveSpawn->GetComponentLocation(), Boss->GetActorRotation(), SpawnActorOption);

		if (AddAttackBall)
		{
			if (BossWaveTarget)
			{
				AddAttackBall->Homing(BossWaveTarget);
			}			
		}		
	}
}

void UBossAnimInstance::AnimNotify_OrbCreate(UAnimNotify * Notify)
{
	GLog->Log(FString::Printf(TEXT("오브 생성")));
	ABoss* Boss = Cast<ABoss>(TryGetPawnOwner());

	if (Boss)
	{
		FActorSpawnParameters SpawnActorOption;
		SpawnActorOption.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

		ABossOrb* Orb = GetWorld()->SpawnActor<ABossOrb>(Orb->StaticClass(), Boss->OrbCreateLocation->GetComponentLocation(), Boss->GetActorRotation(), SpawnActorOption);

		if (Orb)
		{
			Orb->OwnerSave(Boss);
			Boss->OrbMaxCount--;
		}
	}	
}

void UBossAnimInstance::AnimNotify_WaveAttackWaveSpawn(UAnimNotify * Notify)
{
	ABoss* Boss = Cast<ABoss>(TryGetPawnOwner());

	if (Boss)
	{
		FVector LockonTargetLocation;
		FVector Location;
		ABossWaveTarget* BossWaveTarget;
		ABossWaveAttackWave* WaveAttackWave;

		FActorSpawnParameters SpawnActorOption;
		SpawnActorOption.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
		
		LockonTargetLocation = Boss->TargetCamera->GetActorLocation();

		BossWaveTarget = GetWorld()->SpawnActor<ABossWaveTarget>(BossWaveTarget->StaticClass(), LockonTargetLocation, FRotator::ZeroRotator);

		WaveAttackWave = GetWorld()->SpawnActor<ABossWaveAttackWave>(WaveAttackWave->StaticClass(), Boss->SwordWaveSpawn->GetComponentLocation(), Boss->GetActorRotation(), SpawnActorOption);

		if (WaveAttackWave)
		{
			if (BossWaveTarget)
			{
				WaveAttackWave->Homing(BossWaveTarget);
			}
		}
	}
}


