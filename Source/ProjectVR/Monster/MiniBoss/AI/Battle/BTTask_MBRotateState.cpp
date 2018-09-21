// Fill out your copyright notice in the Description page of Project Settings.

#include "BTTask_MBRotateState.h"
#include "Headers/MiniBossAIHeader.h"

void UBTTask_MBRotateState::InitializeFromAsset(UBehaviorTree & Asset)
{
	Super::InitializeFromAsset(Asset);

	bNotifyTick = true;
	InitRotator.ZeroRotator;
}

EBTNodeResult::Type UBTTask_MBRotateState::ExecuteTask(UBehaviorTreeComponent & OwnerComp, uint8 * NodeMemory)
{
	AMiniBossAIController* AI = Cast<AMiniBossAIController>(OwnerComp.GetAIOwner());

	if (AI)
	{
		MiniBoss = Cast<AMiniBoss>(AI->GetPawn());

		if (MiniBoss)
		{
			MyCharacter = Cast<AMotionControllerCharacter>(AI->BBComponent->GetValueAsObject("Player"));

			if (MyCharacter)
			{
				FVector Velocity;
				FRotator TestRotator;
				Velocity =MiniBoss->GetCharacterMovement()->Velocity;
				TestRotator = MiniBoss->GetActorRotation();
				FVector NormalVector = UKismetMathLibrary::Normal(Velocity);
				FRotator XNormalRotator = UKismetMathLibrary::MakeRotFromX(NormalVector);
				FRotator CompleteRotator = UKismetMathLibrary::NormalizedDeltaRotator(TestRotator, XNormalRotator);
				//LookAt = UKismetMathLibrary::FindLookAtRotation(MiniBoss->GetActorLocation(), MyCharacter->Camera->GetComponentLocation());
				//AI->BBComponent->SetValueAsRotator("LookAtRotator", LookAt);				

				//float LookAtYaw = LookAt.Yaw;

				//MiniBoss->RotateYaw = CompleteRotator.Yaw;

				//GLog->Log(FString::Printf(TEXT("%0.1f"),LookAtYaw));
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
		//GLog->Log(FString::Printf(TEXT("Angle")));
		if (UKismetMathLibrary::EqualEqual_RotatorRotator(InitRotator, LookAt, 10.0f))
		{
			//GLog->Log(FString::Printf(TEXT("`Angle 10")));
			//MiniBoss->CurrentAnimState = EMiniBossAnimState::AttackReady;
		}
	}
}
