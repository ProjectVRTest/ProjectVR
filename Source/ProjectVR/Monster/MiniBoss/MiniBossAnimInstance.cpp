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
#include "Math/RandomStream.h"

#define LEFT 1
#define STRAIGHT 2
#define RIGHT 3

void UMiniBossAnimInstance::NativeBeginPlay()
{
	Super::NativeBeginPlay();
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
	FVector LaunchVector;
	FVector RandomYawVector;
	FRotator RandomRotator=FRotator::ZeroRotator;
	int RandomYaw = FMath::RandRange(1, 3);
	if (MiniBoss)
	{
		AMotionControllerCharacter* MyCharacter = Cast<AMotionControllerCharacter>(MiniBoss->Target);
		if (MyCharacter)
		{
			switch (RandomYaw)
			{
			case LEFT:
				RandomRotator.Yaw = -5.0f;
				break;
			case STRAIGHT:
				RandomRotator.Yaw = 0;
				break;
			case RIGHT:
				RandomRotator.Yaw = 5.0f;
				break;
			}

			MiniBoss->SetActorRotation(MiniBoss->GetActorRotation() + RandomRotator);
			MiniBoss->GetCharacterMovement()->GroundFriction = 1.5f;
			MiniBoss->GetMesh()->SetMaterial(0, MiniBoss->OpacityMaterials);
			FVector DistanceVector = UKismetMathLibrary::Subtract_VectorVector(MiniBoss->GetActorLocation(), MyCharacter->Camera->GetComponentLocation());
			LaunchVector = (DistanceVector.Size() * MiniBoss->GetActorForwardVector()*2.5f)+ MiniBoss->GetActorUpVector()*100.0f;
			//GLog->Log(FString::Printf(TEXT("Y : %d"), RandomYaw));
			MiniBoss->LaunchCharacter(LaunchVector, true, true);
		}
		
		
		//MiniBoss->GetCharacterMovement()->AddImpulse((MiniBoss->GetActorForwardVector()*1000.0f) + MiniBoss->GetActorUpVector()*10.0f, true);

		
	}
}

void UMiniBossAnimInstance::AnimNotify_DashEnd(UAnimNotify * Notify)
{
	AMiniBoss* MiniBoss = Cast<AMiniBoss>(TryGetPawnOwner());
	
	if (MiniBoss)
	{
		MiniBoss->GetMesh()->SetMaterial(0, MiniBoss->DefaultMaterials);
	}
}

void UMiniBossAnimInstance::AnimNotify_GroundFrictionDefault(UAnimNotify * Notify)
{
	AMiniBoss* MiniBoss = Cast<AMiniBoss>(TryGetPawnOwner());

	if (MiniBoss)
	{
		MiniBoss->GetCharacterMovement()->GroundFriction = 8.0f;	
	}
}
