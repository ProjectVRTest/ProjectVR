// Fill out your copyright notice in the Description page of Project Settings.

#include "BossAnimInstance.h"
#include "Monster/Boss/Boss.h"
#include "Monster/Boss/AI/AddAttack/BossAddAttackBall.h"
#include "MyCharacter/MotionControllerCharacter.h"
#include "Monster/SwordWaveTarget.h"
#include "MyCharacter/CameraLocation.h"
#include "Monster/Boss/AI/AddAttack/BossAddAttackBall.h"

void UBossAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);

	ABoss* Boss = Cast<ABoss>(TryGetPawnOwner());

	if (Boss && Boss->IsValidLowLevelFast())
	{
		CurrentState = Boss->CurrentState;
		CurrentBlinkAttackState = Boss->CurrentBlinkAttackState;
		CurrentBattleState = Boss->CurrentBattleState;
	}
}

void UBossAnimInstance::AnimNotify_LongDistanceAttackBallSpawn(UAnimNotify * Notify)
{
	ABoss* Boss = Cast<ABoss>(TryGetPawnOwner());
	FVector Location;
	if (Boss)
	{
		FActorSpawnParameters SpawnActorOption;
		SpawnActorOption.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButDontSpawnIfColliding;

		int LeftRightValue = FMath::RandRange(0, 1);

		switch (LeftRightValue)
		{
		case 0:
			Location = FVector(450.0f, 450.0f, 0);
			break;
		case 1:
			Location = FVector(450.0f, -450.0f, 0);
			break;
		}
		
		FVector FirstTargetLocation = Boss->GetActorLocation() + Location;

		FVector LockonTargetLocation = Boss->TargetCamera->GetActorLocation();

		ASwordWaveTarget* SwordWaveTarget = GetWorld()->SpawnActor<ASwordWaveTarget>(SwordWaveTarget->StaticClass(), FirstTargetLocation, FRotator::ZeroRotator);

		ABossAddAttackBall* AddAttackBall = GetWorld()->SpawnActor<ABossAddAttackBall>(AddAttackBall->StaticClass(), Boss->SwordWaveSpawn->GetComponentLocation(), Boss->GetActorRotation(), SpawnActorOption);

		AddAttackBall->Homing(SwordWaveTarget);
	}
}


