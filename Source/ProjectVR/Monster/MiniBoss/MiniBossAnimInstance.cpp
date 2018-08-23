// Fill out your copyright notice in the Description page of Project Settings.

#include "MiniBossAnimInstance.h"
#include "Monster/MiniBoss/MiniBoss.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"

#include "GameFramework/CharacterMovementComponent.h"
#include "MyCharacter/MotionControllerCharacter.h"
#include "Camera/CameraComponent.h"

#include "MyTargetPoint.h"
#include "TimerManager.h"
#include "MyCharacter/MotionControllerPC.h"
#include "MyCharacter/MotionControllerPC.h"

void UMiniBossAnimInstance::NativeBeginPlay()
{
	Super::NativeBeginPlay();

	UE_LOG(LogClass, Warning, TEXT("애니메이션 시작"));

}

void UMiniBossAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);

	AMiniBoss* MiniBoss = Cast<AMiniBoss>(TryGetPawnOwner());
	FVector Velocity;
	FRotator TestRotator;
	if (MiniBoss && MiniBoss->IsValidLowLevelFast())
	{
		CurrentState = MiniBoss->CurrentState;
		CurrentAnimState = MiniBoss->CurrentAnimState;
		CurrentJumpState = MiniBoss->CurrentJumpState;
		CurrentAttackState = MiniBoss->CurrentAttackState;
		CurrentDashState = MiniBoss->CurrentDashState;
		WalkStopFlag = MiniBoss->WalkStopFlag;
		Yaw = MiniBoss->Yaw;
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
		FVector GoalVector = MyCharacter->AttackPoints[0]->GetActorLocation();

		UGameplayStatics::SuggestProjectileVelocity_CustomArc(GetWorld(),
			LaunchVector,
			MiniBoss->GetActorLocation(),
			GoalVector,
			0,
			0.5f);
		//UE_LOG(LogClass, Warning, TEXT("X : %f Y : %f Z :%f"), LaunchVector.X, LaunchVector.Y, LaunchVector.Z);
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

void UMiniBossAnimInstance::AnimNotify_AttackComplete(UAnimNotify * Notify)
{
	AMiniBoss* MiniBoss = Cast<AMiniBoss>(TryGetPawnOwner());

	if (MiniBoss)
	{
		MiniBoss->AttackCompleteFlag = true;
	}
}

void UMiniBossAnimInstance::AnimNotify_DashStart(UAnimNotify * Notify)
{
	AMiniBoss* MiniBoss = Cast<AMiniBoss>(TryGetPawnOwner());

	if (MiniBoss)
	{
		AMotionControllerCharacter* MyCharacter = Cast<AMotionControllerCharacter>(MiniBoss->Target);

		FVector LaunchVector;
		LaunchVector = MiniBoss->GetActorForwardVector()*300.0f + MiniBoss->GetActorUpVector()*2.0f;

		MiniBoss->GetCharacterMovement()->AddImpulse((MiniBoss->GetActorForwardVector()*200.0f) + MiniBoss->GetActorUpVector()*2.0f, true);

		//MiniBoss->LaunchCharacter(LaunchVector, true, true);
	}
}
