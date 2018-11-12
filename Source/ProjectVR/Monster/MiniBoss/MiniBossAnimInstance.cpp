// Fill out your copyright notice in the Description page of Project Settings.

#include "MiniBossAnimInstance.h"
#include "Monster/MiniBoss/MiniBoss.h" //중간보스 헤더파일
#include "Kismet/GameplayStatics.h"   //스폰액터 등 주요한 기능이 있는 헤더파일
#include "Kismet/KismetMathLibrary.h" //각종 수학 관련 함수 헤더파일
#include "GameFramework/CharacterMovementComponent.h" //캐릭터의 속력이 관련된 컴포넌트 헤더파일
#include "MyCharacter/MotionControllerCharacter.h" //내 캐릭터 헤더파일
#include "Monster/MiniBoss/Weapon/SwordWave/MiniBossSwordWave.h" //중간보스 검기 헤더파일
#include "MyCharacter/CameraLocation.h" //내 캐릭터 카메라 헤더파일
#include "Monster/SwordWaveTarget.h" //검기를 락온 시킬때 필요한 액터 헤더파일

void UMiniBossAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);

	AMiniBoss* MiniBoss = Cast<AMiniBoss>(TryGetPawnOwner()); //이 AnimInstance를 사용하고 있는 Pawn을 가지고 온다.

	if (MiniBoss && MiniBoss->IsValidLowLevelFast()) //중간보스가 있고, 중간보스가 유효하면
	{
		//각종 상태값을 업데이트해준다.
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

		AMiniBossSwordWave* SwordWave = GetWorld()->SpawnActor<AMiniBossSwordWave>(SwordWave->StaticClass(), MiniBoss->SwordWaveSpawn->GetComponentLocation(), MiniBoss->GetActorRotation(), SpawnActorOption);
	
		if (SwordWave)
		{
			SwordWave->Homing(SwordWaveTarget);
		}
	}
}