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
		WalkStopFlag = MiniBoss->WalkStopFlag;
		Yaw = MiniBoss->Yaw;
		AMotionControllerPC* PC = Cast<AMotionControllerPC>(UGameplayStatics::GetPlayerController(GetWorld(), 0));		

		if (PC)
		{
			AMotionControllerCharacter* MyCharacter = Cast<AMotionControllerCharacter>(PC->GetPawn());

			if (MyCharacter)
			{
				FRotator LookAt, CurrentRot;

				/*LookAt = UKismetMathLibrary::FindLookAtRotation(MiniBoss->GetActorLocation(), MyCharacter->GetActorLocation());
				CurrentRot = FMath::Lerp(MiniBoss->GetActorRotation(), LookAt, DeltaSeconds);

				MiniBoss->SetActorRotation(CurrentRot);*/		

				//Velocity = MiniBoss->GetCharacterMovement()->Velocity;
				//TestRotator = MiniBoss->GetActorRotation();

				//FVector NormalVector = UKismetMathLibrary::Normal(Velocity);
				//FRotator XNormalRotator=UKismetMathLibrary::MakeRotFromX(NormalVector);
				//FRotator CompleteRotator = UKismetMathLibrary::NormalizedDeltaRotator(TestRotator, XNormalRotator);
				//Yaw = CompleteRotator.Yaw;

				//UE_LOG(LogClass, Warning, TEXT("Yaw : %f"), Yaw);
			}
		}

		Direction = CalculateDirection(MiniBoss->GetCharacterMovement()->Velocity, MiniBoss->GetActorRotation());
		Speed = MiniBoss->GetCharacterMovement()->Velocity.Size();
		
		//UE_LOG(LogClass, Warning, TEXT("Direction : %0.1f\n Speed : %0.1f\n"), Direction, Speed);
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
		Miniboss->CurrentJumpState = EMiniBossJumpState::Idle;
		Miniboss->CurrentAnimState = EMiniBossAnimState::Wait;
	}
}

void UMiniBossAnimInstance::AnimNotify_AttackComplete(UAnimNotify * Notify)
{
	AMiniBoss* Miniboss = Cast<AMiniBoss>(TryGetPawnOwner());

	if (Miniboss)
	{
		Miniboss->AttackCompleteFlag = true;
	}
}
