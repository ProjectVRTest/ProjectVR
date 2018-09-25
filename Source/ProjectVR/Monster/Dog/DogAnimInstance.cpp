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

	// 계산 중복 방지 - 공중판단 지움
	if (RagdollDog && RagdollDog->IsValidLowLevelFast())
	{
		CurrentDogState = RagdollDog->CurrentDogState;
		CurrentDogAnimState = RagdollDog->CurrentDogAnimState;
		CurrentDogBattleState = RagdollDog->CurrentDogBattleState;
		CurrentDogJumpState = RagdollDog->CurrentDogJumpState;
		CurrentDogCircleState = RagdollDog->CurrentDogCircleState;
		CurrentDogAirState = RagdollDog->CurrentDogAirState;
	}
}

// 점프 공격
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
			RagdollDog->DogAttackCollision->bGenerateOverlapEvents = true;
			RagdollDog->LaunchCharacter(LaunchVector, true, true);
			RagdollDog->bOnLand = false;		// 땅에 없음
		}
	}
}
