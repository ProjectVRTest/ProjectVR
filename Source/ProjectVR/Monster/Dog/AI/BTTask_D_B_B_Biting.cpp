// Fill out your copyright notice in the Description page of Project Settings.

#include "BTTask_D_B_B_Biting.h"

#include "Monster/Dog/DogAIController.h"
#include "Monster/Dog/Dog.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Components/CapsuleComponent.h"								// 땅과의 거리 판단을 위해서 필요한 컴포넌트
#include "Components/BoxComponent.h"									// 입 콜리전 활성화/비활성화를 위한 컴포넌트
#include "Components/SkeletalMeshComponent.h"						// 개의 래그돌화를 위해서 필요한 컴포넌트
#include "BehaviorTree/BlackboardComponent.h"							// 커스텀 대기시간
#include "HandMotionController/RightHandMotionController.h"
#include "MyCharacter/MotionControllerCharacter.h"
#include "Components/CapsuleComponent.h"
#include "Camera/CameraComponent.h"

void UBTTask_D_B_B_Biting::InitializeFromAsset(UBehaviorTree & Asset)
{
	Super::InitializeFromAsset(Asset);

	bNotifyTick = true;				// 틱을 실행할 것인지 

}

EBTNodeResult::Type UBTTask_D_B_B_Biting::ExecuteTask(UBehaviorTreeComponent & OwnerComp, uint8 * NodeMemory)
{
	// Dog 캐스트 작업
	AI = Cast<ADogAIController>(OwnerComp.GetAIOwner());
	if (AI)
	{
		Dog = Cast<ADog>(AI->GetPawn());

		if (AI->BBComponent->GetValueAsObject("AttachActor"))
			RightController = Cast<ARightHandMotionController>(AI->BBComponent->GetValueAsObject("AttachActor"));
		if (RightController)
			Player = Cast<AMotionControllerCharacter>(RightController->HandOwner);
	}

	return EBTNodeResult::InProgress;			// Tick
}

void UBTTask_D_B_B_Biting::TickTask(UBehaviorTreeComponent & OwnerComp, uint8 * NodeMemory, float DeltaSeconds)
{
	Super::TickTask(OwnerComp, NodeMemory, DeltaSeconds);

	if (Dog->AttachActor && Player)
	{

		Dog->SetActorRelativeRotation(FRotator(0.0f, 0.0f, 0.0f));
		Dog->SetActorRelativeLocation(FVector(0.0f, 0.0f, 0.0f));

		// 일정 스택 이상일 때와 체력이 0이하일 때, 떼어내는 조건
		if (RightController->stack >= MaxStack || AI->BBComponent->GetValueAsFloat("HP") <= 0)
		{
			// 날라가면 다음 개가 공격할 수 있도록 배열에서 뺌
			if (Player->DogArray.Contains(Dog))			// 배열에 개가 있으면
				Player->DogArray.Remove(Dog);				// 제거

			Dog->DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);		// 뗌

			// 날라가는 방향
			FVector Direction = Player->Camera->GetUpVector() + Player->Camera->GetForwardVector();

			// 날라가는 힘을 조절
			Dog->GetCapsuleComponent()->SetPhysicsLinearVelocity(Direction* 500.0f);
			Dog->GetCapsuleComponent()->SetPhysicsAngularVelocity(Direction* 500.0f);
			Dog->GetCapsuleComponent()->SetSimulatePhysics(true);
			Dog->GetCapsuleComponent()->AddForce(Direction * 500.0f);

			Dog->GetMesh()->SetAllBodiesBelowSimulatePhysics("Bip002-Neck", true, true);		// 힘 빼줌
			Dog->CurrentDogBattleState = EDogBattleState::Air;				// 공중 상태
			
			AI->BBComponent->SetValueAsBool("bIsBiting", false);
			RightController->stack = 0;
			RightController->AttachDog = nullptr;

			FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);				// 틱 종료
		}
	}
}
