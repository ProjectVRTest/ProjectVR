// Fill out your copyright notice in the Description page of Project Settings.

#include "BTTask_MBRotateState.h"
#include "Headers/MiniBossAIHeader.h"
#include "MyCharacter/CameraLocation.h"

void UBTTask_MBRotateState::InitializeFromAsset(UBehaviorTree & Asset)
{
	Super::InitializeFromAsset(Asset);

	bNotifyTick = true;
}

EBTNodeResult::Type UBTTask_MBRotateState::ExecuteTask(UBehaviorTreeComponent & OwnerComp, uint8 * NodeMemory)
{
	AMiniBossAIController* AI = Cast<AMiniBossAIController>(OwnerComp.GetAIOwner());

	if (AI)
	{
		MiniBoss = Cast<AMiniBoss>(AI->GetPawn());

		if (MiniBoss)
		{
			ACameraLocation* CameraLocation = Cast<ACameraLocation>(AI->BBComponent->GetValueAsObject("PlayerCamera"));

			if (CameraLocation)
			{
				LookAt= UKismetMathLibrary::NormalizedDeltaRotator(MiniBoss->GetActorRotation(), UKismetMathLibrary::FindLookAtRotation(MiniBoss->GetActorLocation(), CameraLocation->GetActorLocation()));
				AI->BBComponent->SetValueAsRotator("LookAtRotator", LookAt);
				MiniBoss->RotateYaw = LookAt.Yaw*-1.0f;
				GLog->Log(FString::Printf(TEXT("RotateYaw %f"), MiniBoss->RotateYaw));
			}
		}		
	}

	return EBTNodeResult::InProgress;
}

void UBTTask_MBRotateState::TickTask(UBehaviorTreeComponent & OwnerComp, uint8 * NodeMemory, float DeltaSeconds)
{
	Super::TickTask(OwnerComp, NodeMemory, DeltaSeconds);

	if (MiniBoss)
	{
		FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
	}
}
