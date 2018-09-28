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
#include "Weapon/SwordWave/SwordWaveTarget.h"

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
		CurrentShortAttackState = MiniBoss->CurrentShortAttackState;
		CurrentWaveAttackState = MiniBoss->CurrentWaveAttackState;
		CurrentComboAttackState = MiniBoss->CurrentComboAttackState;
		CurrentBackAttackState = MiniBoss->CurrentBackAttackState;
		CurrentDashState = MiniBoss->CurrentDashState;
		CurrentParryingState = MiniBoss->CurrentParryingState;

		WalkStopFlag = MiniBoss->WalkStopFlag;
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
		FVector GoalVector = MyCharacter->AttackPoints[0]->GetActorLocation();
		float CalculateLaunchVector = GoalVector.Size() - MiniBoss->GetActorLocation().Size();
		UGameplayStatics::SuggestProjectileVelocity_CustomArc(GetWorld(),
			LaunchVector,
			MiniBoss->GetActorLocation(),
			GoalVector,
			0,
			0.65f);

		FHitResult outHit;
		TArray<FVector> Vector;
		FVector OutVector;
		TArray<TEnumAsByte<EObjectTypeQuery>> ObjType;
		TArray<AActor*> IgnoreActor;

		IgnoreActor.Add(MiniBoss);

		UGameplayStatics::PredictProjectilePath(
			GetWorld(),
			outHit,
			Vector,
			OutVector,
			MiniBoss->GetActorLocation(),
			LaunchVector,
			true,
			10.0f,
			ObjType,
			false,
			IgnoreActor,
			EDrawDebugTrace::Persistent,
			0,
			15.0f,
			2.0f,
			0);
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

void UMiniBossAnimInstance::AnimNotify_SwordWaveSpawn(UAnimNotify * Notify)
{
	AMiniBoss* MiniBoss = Cast<AMiniBoss>(TryGetPawnOwner());
	
	if (MiniBoss)
	{
		AMiniBossWeapon* MiniBossWeapon = MiniBoss->Sword;

		FActorSpawnParameters SpawnActorOption;
		SpawnActorOption.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButDontSpawnIfColliding;

		if (MiniBossWeapon)
		{
			FVector LockonTargetLocation = MiniBoss->TargetCamera->GetActorLocation();

			ASwordWaveTarget* SwordWaveTarget = GetWorld()->SpawnActor<ASwordWaveTarget>(SwordWaveTarget->StaticClass(), LockonTargetLocation,FRotator::ZeroRotator);
		
			ASwordWave* SwordWave = GetWorld()->SpawnActor<ASwordWave>(SwordWave->StaticClass(), MiniBoss->SwordWaveSpawn->GetComponentLocation(), MiniBoss->GetActorRotation(), SpawnActorOption);
						
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
