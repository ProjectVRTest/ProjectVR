// Fill out your copyright notice in the Description page of Project Settings.

#include "BTTask_WanderForTarget.h"

#include "Monster/Dog/Dog.h"
#include "Monster/Dog/DogAIController.h"
#include "MyCharacter/MotionControllerCharacter.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "Camera/CameraComponent.h"

EBTNodeResult::Type UBTTask_WanderForTarget::ExecuteTask(UBehaviorTreeComponent & OwnerComp, uint8 * NodeMemory)
{
	Super::ExecuteTask(OwnerComp, NodeMemory);

	ADogAIController* AI = Cast<ADogAIController>(OwnerComp.GetAIOwner());
	//time = 1.0f;

	if (AI)
	{
		AActor* Player = Cast<AActor>(AI->BBComponent->GetValueAsObject(TEXT("Player")));
		AMotionControllerCharacter* MyCharacter = Cast<AMotionControllerCharacter>(Player);
		ADog* RagdollDog = Cast<ADog>(AI->GetPawn());

		int Rotcheck;

		if (RagdollDog && MyCharacter)
		{
			if (!RagdollDog->AttachActor)
			{
				Rotcheck = AI->BBComponent->GetValueAsInt("RotateCheck");

				if (Rotcheck == 1)
				{
					time = 1.0f;
					//RagdollDog->CurrentDogAnimState = EDogAnimState::RightSideWalk;
				}
				else if (Rotcheck == 2)
				{
					time = -1.0f;
					//RagdollDog->CurrentDogAnimState = EDogAnimState::LeftSideWalk;
				}
				FVector Vec1 = UKismetMathLibrary::Subtract_VectorVector(RagdollDog->GetActorLocation(), MyCharacter->Camera->GetComponentLocation());
				FVector Vec2 = UKismetMathLibrary::RotateAngleAxis(Vec1, time, FVector(0.0f, 0.0f, 1.0f));
				
				FVector Vec3 = MyCharacter->Camera->GetComponentLocation() + Vec2;

				FRotator Rot1 = RagdollDog->GetActorRotation() + UKismetMathLibrary::RotatorFromAxisAndAngle(FVector(0.0f, 0.0f, 1.0f), time);

				RagdollDog->SetActorLocationAndRotation(Vec3, Rot1);
				return EBTNodeResult::Succeeded;
			}
		}
	}

	return EBTNodeResult::Failed;
}

void UBTTask_WanderForTarget::InitializeFromAsset(UBehaviorTree & Asset)
{
	Super::InitializeFromAsset(Asset);

	time = 0.0f;
}
