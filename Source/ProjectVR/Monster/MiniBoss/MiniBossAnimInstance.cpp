// Fill out your copyright notice in the Description page of Project Settings.

#include "MiniBossAnimInstance.h"
#include "Monster/MiniBoss/MiniBoss.h"
#include "Kismet/GameplayStatics.h"

#include "GameFramework/CharacterMovementComponent.h"
#include "MyCharacter/MotionControllerCharacter.h"
#include "Camera/CameraComponent.h"

#include "MyTargetPoint.h"
#include "TimerManager.h"

void UMiniBossAnimInstance::NativeBeginPlay()
{
	Super::NativeBeginPlay();

	UE_LOG(LogClass, Warning, TEXT("애니메이션 시작"));

	
}

void UMiniBossAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);

	AMiniBoss* Miniboss = Cast<AMiniBoss>(TryGetPawnOwner());

	if (Miniboss && Miniboss->IsValidLowLevelFast())
	{
		CurrentState = Miniboss->CurrentState;
		CurrentAnimState = Miniboss->CurrentAnimState;
		CurrentJumpState = Miniboss->CurrentJumpState;
		CurrentAttackState = Miniboss->CurrentAttackState;

		Direction = CalculateDirection(Miniboss->GetCharacterMovement()->Velocity, Miniboss->GetActorRotation());
		Speed = Miniboss->GetCharacterMovement()->Velocity.Size();
	}
}

void UMiniBossAnimInstance::AnimNotify_JumpAttackStart(UAnimNotify * Notify)
{
	AMiniBoss* Miniboss = Cast<AMiniBoss>(TryGetPawnOwner());
		
	if (Miniboss)
	{
		AMotionControllerCharacter* MyCharacter = Cast<AMotionControllerCharacter>(Miniboss->Target);
		FVector LaunchVector;
		FVector GoalVector = MyCharacter->AttackPoints[0]->GetActorLocation();

		UGameplayStatics::SuggestProjectileVelocity_CustomArc(GetWorld(),
			LaunchVector,
			Miniboss->GetActorLocation(),
			GoalVector,
			0,
			0.5f);
		UE_LOG(LogClass, Warning, TEXT("X : %f Y : %f Z :%f"), LaunchVector.X, LaunchVector.Y, LaunchVector.Z);
		Miniboss->LaunchCharacter(LaunchVector, true, true);
	}
}

void UMiniBossAnimInstance::AnimNotify_JumpAttackEnd(UAnimNotify * Notify)
{
	AMiniBoss* Miniboss = Cast<AMiniBoss>(TryGetPawnOwner());

	if (Miniboss)
	{
		Miniboss->CurrentJumpState = EMonsterJumpState::Idle;
		Miniboss->CurrentAnimState = EMonsterAnimState::Run;
	}
}
