// Fill out your copyright notice in the Description page of Project Settings.

#include "BossAnimInstance.h"
#include "Monster/Boss/Boss.h"
#include "Monster/Boss/AI/AddAttack/BossAddAttackBall.h"
#include "MyCharacter/MotionControllerCharacter.h"
#include "MyCharacter/CameraLocation.h"
#include "Monster/Boss/AI/AddAttack/BossAddAttackBall.h"
#include "Components/SphereComponent.h"
#include "BossWaveTarget.h"
#include "Orb/BossOrb.h"

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
	}
}

void UBossAnimInstance::AnimNotify_LongDistanceAttackBallSpawn(UAnimNotify * Notify)
{
	ABoss* Boss = Cast<ABoss>(TryGetPawnOwner());
	
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
	ABoss* Boss = Cast<ABoss>(TryGetPawnOwner());

	if (Boss)
	{
		FActorSpawnParameters SpawnActorOption;
		SpawnActorOption.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

		ABossOrb* Orb = GetWorld()->SpawnActor<ABossOrb>(Orb->StaticClass(), Boss->OrbCreateLocation->GetComponentLocation(), Boss->GetActorRotation(), SpawnActorOption);

		if (Orb)
		{
			Boss->OrbMaxCount--;
		}
	}	
}


