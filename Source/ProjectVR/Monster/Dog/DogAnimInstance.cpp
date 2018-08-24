// Fill out your copyright notice in the Description page of Project Settings.

#include "DogAnimInstance.h"
#include "Monster/Dog/Dog.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "MyCharacter/MotionControllerCharacter.h"
#include "HandMotionController/RightHandMotionController.h"
#include "Components/CapsuleComponent.h"
#include "Camera/CameraComponent.h"
#include "Components/BoxComponent.h"

void UDogAnimInstance::NativeInitializeAnimation()
{
	Super::NativeInitializeAnimation();

	CurrentFalling = false;
	PreviousFalling = false;
}

void UDogAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);

	ADog* RagdollDog = Cast<ADog>(TryGetPawnOwner());
	FFindFloorResult FloorDistance;;

	if (RagdollDog && RagdollDog->IsValidLowLevelFast())
	{
		CurrentDogState = RagdollDog->CurrentDogState;
		CurrentDogAnimState = RagdollDog->CurrentDogAnimState;
		CurrentDogJumpState = RagdollDog->CurrentDogJumpState;
		CurrentDogCircleState = RagdollDog->CurrentDogCircleState;

		CurrentFalling = RagdollDog->CurrentFalling;

		if (CurrentFalling && PreviousFalling)
		{
			RagdollDog->CurrentDogJumpState = EDogJumpState::JumpRoof;
		}

		if (!CurrentFalling && PreviousFalling)
		{
			RagdollDog->GetCharacterMovement()->ComputeFloorDist(RagdollDog->GetCapsuleComponent()->GetComponentLocation(), 10000.0f, 10000.0f, FloorDistance, 34, 0);

			if (FloorDistance.FloorDist < 3.0f)
			{
				RagdollDog->bOnLand = true;		// ¶¥¿¡ ÀÖÀ½

				if (RagdollDog->bIsDeath)
				{
					RagdollDog->CurrentDogState = EDogState::Death;
					RagdollDog->CurrentDogAnimState = EDogAnimState::Nothing;
					RagdollDog->CurrentDogJumpState = EDogJumpState::Nothing;
					RagdollDog->CurrentDogCircleState = EDogCircleState::Nothing;

					PreviousFalling = CurrentFalling;
					return;
				}
				else if (RagdollDog->bIsDetach)
				{
					RagdollDog->CurrentDogState = EDogState::Hurled;
					RagdollDog->CurrentDogAnimState = EDogAnimState::StandUp;
					RagdollDog->CurrentDogJumpState = EDogJumpState::Nothing;
					RagdollDog->CurrentDogCircleState = EDogCircleState::Nothing;

					RagdollDog->bIsDetach = false;

					PreviousFalling = CurrentFalling;
					return;
				}
				
				RagdollDog->Landing = true;
			}
			
			
		}
		PreviousFalling = CurrentFalling;
	}
}

void UDogAnimInstance::AnimNotify_JumpStart(UAnimNotify * Notify)
{
	ADog* RagdollDog = Cast<ADog>(TryGetPawnOwner());

	if (RagdollDog)
	{
		AMotionControllerCharacter* Target = Cast<AMotionControllerCharacter>(RagdollDog->Target);

		if (Target)
		{
			RagdollDog->bIsAttack = true;
			RagdollDog->DogAttackCollision->SetActive(true);
			FVector LaunchVector;
			FVector GoalVector = Target->Camera->GetComponentLocation();
			UGameplayStatics::SuggestProjectileVelocity_CustomArc(GetWorld(),
				LaunchVector,
				RagdollDog->GetActorLocation(),
				GoalVector,
				0,
				0.5f);
			RagdollDog->LaunchCharacter(LaunchVector, true, true);
			RagdollDog->bOnLand = false;		// ¶¥¿¡ ¾øÀ½
		}
	}
}
