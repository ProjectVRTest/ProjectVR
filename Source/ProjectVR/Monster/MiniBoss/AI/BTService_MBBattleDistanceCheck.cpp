// Fill out your copyright notice in the Description page of Project Settings.

#include "BTService_MBBattleDistanceCheck.h"
#include "Headers/MiniBossAIHeader.h"

void UBTService_MBBattleDistanceCheck::TickNode(UBehaviorTreeComponent & OwnerComp, uint8 * NodeMemory, float DeltaSeconds)
{
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);

	FName DistanceBlackBoardKey = GetSelectedBlackboardKey();

	AMiniBossAIController* AI = Cast<AMiniBossAIController>(OwnerComp.GetAIOwner());

	if (AI)
	{
		FRotator LookAt, CurrentRot;
		FVector Velocity;
		FRotator TestRotator;
		//float Yaw;
		Distance = AI->BBComponent->GetValueAsFloat(DistanceBlackBoardKey);
		AActor* Player = Cast<AActor>(AI->BBComponent->GetValueAsObject(TEXT("Player")));

		if (Player)
		{
			AMotionControllerCharacter* MyCharacter = Cast<AMotionControllerCharacter>(Player);

			if (MyCharacter)
			{
				AMiniBoss* MiniBoss = Cast<AMiniBoss>(AI->GetPawn());

				LookAt = UKismetMathLibrary::FindLookAtRotation(MiniBoss->GetActorLocation(), MyCharacter->GetActorLocation());
				CurrentRot = FMath::Lerp(MiniBoss->GetActorRotation(), LookAt, DeltaSeconds);

				MiniBoss->SetActorRotation(CurrentRot);

				Velocity = MiniBoss->GetCharacterMovement()->Velocity;
				TestRotator = MiniBoss->GetActorRotation();

				FVector NormalVector = UKismetMathLibrary::Normal(Velocity);
				FRotator XNormalRotator = UKismetMathLibrary::MakeRotFromX(NormalVector);
				FRotator CompleteRotator = UKismetMathLibrary::NormalizedDeltaRotator(TestRotator, XNormalRotator);
				MiniBoss->Yaw = CompleteRotator.Yaw;
		
				GLog->Log(FString::Printf(TEXT("Yaw : %0.1f"), MiniBoss->Yaw));
				if (MiniBoss)
				{
					if (Distance > 500.0f)
					{
						MiniBoss->WalkStopFlag = false;
						MiniBoss->CurrentState = EMiniBossState::Chase;
						MiniBoss->CurrentAnimState = EMiniBossAnimState::Walk;
					}

					if (Distance < 300.0f)
					{
						//MiniBoss->CurrentAnimState = EMiniBossAnimState::Attack;
					}
				}
			}
		}	
	}
}
