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

		CurrentFalling = RagdollDog->CurrentFalling;

		if (RagdollDog->AttachActor)
		{
			RagdollDog->CurrentDogState = EDogState::Battle;
			RagdollDog->CurrentDogAnimState = EDogAnimState::JumpAttack;
			RagdollDog->CurrentDogJumpState = EDogJumpState::JumpRoof;
		}
		//LookAtRotator = RagdollDog->LookAtRotator;

		if (CurrentFalling && PreviousFalling)
		{
			RagdollDog->CurrentDogJumpState = EDogJumpState::JumpRoof;
		}

		if (!CurrentFalling && PreviousFalling)
		{
			RagdollDog->GetCharacterMovement()->ComputeFloorDist(RagdollDog->GetCapsuleComponent()->GetComponentLocation(), 10000.0f, 10000.0f, FloorDistance, 34, 0);

			if (FloorDistance.FloorDist < 200.0f)
			{
				//UE_LOG(LogClass, Warning, TEXT("Zennaro KaTuSSo"));
				RagdollDog->CurrentDogState = EDogState::Chase;
				RagdollDog->CurrentDogAnimState = EDogAnimState::Run;
				RagdollDog->CurrentDogJumpState = EDogJumpState::Nothing;

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
			RagdollDog->DogAttackCollision->SetActive(true);
			FVector LaunchVector;
			FVector GoalVector = Target->Camera->GetComponentLocation();
			UGameplayStatics::SuggestProjectileVelocity_CustomArc(GetWorld(),
				LaunchVector,
				RagdollDog->GetActorLocation(),
				GoalVector,
				0,
				0.5f);
			UE_LOG(LogClass, Warning, TEXT("X : %f Y : %f Z :%f"), LaunchVector.X, LaunchVector.Y, LaunchVector.Z);
			RagdollDog->LaunchCharacter(LaunchVector, true, true);
		}
	}
}
