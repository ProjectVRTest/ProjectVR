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

//점프 공격 시작을 알려주는 노티파이 이벤트
void UMiniBossAnimInstance::AnimNotify_JumpAttackStart(UAnimNotify * Notify)
{
	AMiniBoss* MiniBoss = Cast<AMiniBoss>(TryGetPawnOwner()); //이 애니메이션을 사용하고 있는 폰을 가져온다.

	if (MiniBoss) //중간보스가 있으면
	{
		//점프할 대상을 가져온다.
		AMotionControllerCharacter* MyCharacter = Cast<AMotionControllerCharacter>(MiniBoss->Target);
		FVector LaunchVector; //캐릭터가 점프 할때 필요한 벡터값을 저장하기 위한 변수
		FVector GoalVector = MyCharacter->CameraLocation->GetActorLocation(); //목표지점을 가져온다.
		GoalVector = GoalVector + FVector(150.0f, 0, 0); //목표 지점에서 앞쪽 방향으로 150.0f 만큼 이동한 곳을 최종 점프 목표지점으로 설정한다.
		
		UGameplayStatics::SuggestProjectileVelocity_CustomArc(GetWorld(),
			LaunchVector,
			MiniBoss->GetActorLocation(), //중간보스의 현재 위치부터
			GoalVector, // 최종 목표지점의 위치까지의 벡터값(곡사)을 구해서 LaunchVector에 저장한다.
			0,
			0.65f);
		MiniBoss->LaunchCharacter(LaunchVector, true, true); //앞에서 구한 벡터값만큼 캐릭터를 발사한다.
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

//웨이브 공격 검기를 스폰해주는 노티파이 이벤트
void UMiniBossAnimInstance::AnimNotify_SwordWaveSpawn(UAnimNotify * Notify)
{
	AMiniBoss* MiniBoss = Cast<AMiniBoss>(TryGetPawnOwner()); //이 애니메이션을 사용하고 있는 폰을 가져온다.

	if (MiniBoss) //중간보스가 있으면
	{
		//중간보스 검기 웨이브의 스폰 조건을 설정한다.
		FActorSpawnParameters SpawnActorOption;
		//기본적으로 충돌하지 않는곳을 찾고, 찾지 못할 경우 스폰시키지 않게 스폰 조건을 설정한다.
		SpawnActorOption.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButDontSpawnIfColliding;

		//타겟의 위치를 설정한다.
		FVector LockonTargetLocation = MiniBoss->TargetCamera->GetActorLocation();

		//타겟의 위치에 타겟액터를 스폰한다.
		ASwordWaveTarget* SwordWaveTarget = GetWorld()->SpawnActor<ASwordWaveTarget>(SwordWaveTarget->StaticClass(), LockonTargetLocation, FRotator::ZeroRotator);

		if (SwordWaveTarget) //타겟액터가 있으면
		{
			//검기 웨이브를 스폰한다.
			AMiniBossSwordWave* SwordWave = GetWorld()->SpawnActor<AMiniBossSwordWave>(SwordWave->StaticClass(), MiniBoss->SwordWaveSpawn->GetComponentLocation(), MiniBoss->GetActorRotation(), SpawnActorOption);

			if (SwordWave) //검기 웨이브가 있으면
			{
				//검기 웨이브가 타겟액터를 유도할 수 있도록 해준다.
				SwordWave->Homing(SwordWaveTarget);
			}
		}		
	}
}