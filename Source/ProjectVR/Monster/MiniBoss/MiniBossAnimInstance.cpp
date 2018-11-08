// Fill out your copyright notice in the Description page of Project Settings.

#include "MiniBossAnimInstance.h"
#include "Monster/MiniBoss/MiniBoss.h" //중간보스 헤더파일
#include "Kismet/GameplayStatics.h" 
#include "Kismet/KismetMathLibrary.h" //각종 수학 관련 함수 헤더파일

#include "GameFramework/CharacterMovementComponent.h"
#include "MyCharacter/MotionControllerCharacter.h"
#include "Camera/CameraComponent.h"

#include "MyTargetPoint.h"
#include "TimerManager.h"
#include "MyCharacter/MotionControllerPC.h"

#include "Monster/MiniBoss/Weapon/SwordWave/SwordWave.h"
#include "Monster/MiniBoss/Weapon/MiniBossWeapon.h"
#include "Components/StaticMeshComponent.h"
#include "MyCharacter/CameraLocation.h"
#include "Monster/SwordWaveTarget.h"

void UMiniBossAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);

	AMiniBoss* MiniBoss = Cast<AMiniBoss>(TryGetPawnOwner());

	if (MiniBoss && MiniBoss->IsValidLowLevelFast())
	{
		CurrentState = MiniBoss->CurrentState;
		CurrentAnimState = MiniBoss->CurrentAnimState;
		CurrentJumpState = MiniBoss->CurrentJumpState;
		CurrentAttackState = MiniBoss->CurrentAttackState;
		CurrentShortAttackState = MiniBoss->CurrentShortAttackState;
		CurrentWaveAttackState = MiniBoss->CurrentWaveAttackState;
		CurrentComboAttackState = MiniBoss->CurrentComboAttackState;
		CurrentBackAttackState = MiniBoss->CurrentBackAttackState;
		CurrentParryingState = MiniBoss->CurrentParryingState;

		Yaw = MiniBoss->Yaw;
		RotateYaw = MiniBoss->RotateYaw;
		StabFlag = MiniBoss->StabFlag;
		TwoHandWidthFlag = MiniBoss->TwoHandWidthFlag;
		Speed = MiniBoss->GetCharacterMovement()->Velocity.Size();
	}
}

void UMiniBossAnimInstance::AnimNotify_JumpAttackStart(UAnimNotify * Notify)
{
	AMiniBoss* MiniBoss = Cast<AMiniBoss>(TryGetPawnOwner());

	if (MiniBoss)
	{
		AMotionControllerCharacter* MyCharacter = Cast<AMotionControllerCharacter>(MiniBoss->Target);
		FVector LaunchVector;
		FVector GoalVector = MyCharacter->CameraLocation->GetActorLocation();
		GoalVector = GoalVector + FVector(150.0f, 0, 0);
		float CalculateLaunchVector = GoalVector.Size() - MiniBoss->GetActorLocation().Size();

		UGameplayStatics::SuggestProjectileVelocity_CustomArc(GetWorld(),
			LaunchVector,
			MiniBoss->GetActorLocation(),
			GoalVector,
			0,
			0.65f);
		MiniBoss->LaunchCharacter(LaunchVector, true, true);
	}
}

void UMiniBossAnimInstance::AnimNotify_JumpAttackEnd(UAnimNotify * Notify)
{
	AMiniBoss* MiniBoss = Cast<AMiniBoss>(TryGetPawnOwner());

	if (MiniBoss)
	{
		MiniBoss->CurrentJumpState = EMiniBossJumpState::Idle;
		MiniBoss->CurrentAnimState = EMiniBossAnimState::Wait;
	}
}

void UMiniBossAnimInstance::AnimNotify_SwordWaveSpawn(UAnimNotify * Notify)
{
	AMiniBoss* MiniBoss = Cast<AMiniBoss>(TryGetPawnOwner());

	if (MiniBoss)
	{
		FActorSpawnParameters SpawnActorOption;
		SpawnActorOption.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButDontSpawnIfColliding;

		FVector LockonTargetLocation = MiniBoss->TargetCamera->GetActorLocation();

		ASwordWaveTarget* SwordWaveTarget = GetWorld()->SpawnActor<ASwordWaveTarget>(SwordWaveTarget->StaticClass(), LockonTargetLocation, FRotator::ZeroRotator);

		ASwordWave* SwordWave = GetWorld()->SpawnActor<ASwordWave>(SwordWave->StaticClass(), MiniBoss->SwordWaveSpawn->GetComponentLocation(), MiniBoss->GetActorRotation(), SpawnActorOption);

		if (SwordWave)
		{
			switch (MiniBoss->SwordWaveCount)
			{
			case 1:
				MiniBoss->SwordWaveCount += 1;
				break;
			case 2:
				FRotator NewRotator;
				NewRotator.Roll = 90.0f;
				NewRotator.Pitch = -20.0f;
				NewRotator.Yaw = 90.0f;
				SwordWave->SwordWaveRotatorModify(NewRotator);
				MiniBoss->SwordWaveCount = 1;
				break;
			}
			SwordWave->Homing(SwordWaveTarget);
		}		
	}
}