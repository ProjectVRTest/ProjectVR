// Fill out your copyright notice in the Description page of Project Settings.

#include "BTTask_RotationUpdate.h"
#include "Monster/MiniBoss/MiniBossAIController.h"
#include "Monster/MiniBoss/MiniBoss.h"

#include "BehaviorTree/BlackboardComponent.h"
#include "MyCharacter/MotionControllerCharacter.h"
#include "Kismet/KismetMathLibrary.h"

void UBTTask_RotationUpdate::InitializeFromAsset(UBehaviorTree & Asset)
{
	Super::InitializeFromAsset(Asset);

	bNotifyTick = true;
}

EBTNodeResult::Type UBTTask_RotationUpdate::ExecuteTask(UBehaviorTreeComponent & OwnerComp, uint8 * NodeMemory)
{

	return EBTNodeResult::Type();
}

void UBTTask_RotationUpdate::TickTask(UBehaviorTreeComponent & OwnerComp, uint8 * NodeMemory, float DeltaSeconds)
{
	Super::TickTask(OwnerComp, NodeMemory, DeltaSeconds);

	AMiniBossAIController* AI = Cast<AMiniBossAIController>(OwnerComp.GetAIOwner());

	if (AI)
	{
		AMiniBoss* MiniBoss = Cast<AMiniBoss>(AI->GetPawn());

		AActor* Player = Cast<AActor>(AI->BBComponent->GetValueAsObject(TEXT("Player")));

		if (MiniBoss && Player)
		{
			AMotionControllerCharacter* MyCharacter = Cast<AMotionControllerCharacter>(Player);

			FRotator LookAt, CurrentRot;

			if (MyCharacter)
			{
				LookAt = UKismetMathLibrary::FindLookAtRotation(MiniBoss->GetActorLocation(), MyCharacter->GetActorLocation());
				CurrentRot = FMath::Lerp(MiniBoss->GetActorRotation(), LookAt, 0.1f);

				MiniBoss->CurrentAnimState = EMiniBossAnimState::Wait;
				MiniBoss->SetActorRotation(CurrentRot);

				if (UKismetMathLibrary::EqualEqual_RotatorRotator(CurrentRot, LookAt, 10.0f))
				{
					MiniBoss->CurrentState = EMiniBossState::Battle;
					//MiniBoss->CurrentAnimState = EMiniBossAnimState::AttackReady;
				}
			}
		}
	}
}
