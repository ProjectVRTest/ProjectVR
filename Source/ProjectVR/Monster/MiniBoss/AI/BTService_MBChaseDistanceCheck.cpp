// Fill out your copyright notice in the Description page of Project Settings.

#include "BTService_MBChaseDistanceCheck.h"
#include "Headers/MiniBossAIHeader.h"

void UBTService_MBChaseDistanceCheck::TickNode(UBehaviorTreeComponent & OwnerComp, uint8 * NodeMemory, float DeltaSeconds)
{
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);

	FName DistanceBlackBoardKey = GetSelectedBlackboardKey();

	AMiniBossAIController* AI = Cast<AMiniBossAIController>(OwnerComp.GetAIOwner());

	if (AI)
	{
		FRotator LookAt;
		Distance = AI->BBComponent->GetValueAsFloat(DistanceBlackBoardKey);
		AActor* Player = Cast<AActor>(AI->BBComponent->GetValueAsObject(TEXT("Player")));
		AMotionControllerCharacter* MyCharacter = Cast<AMotionControllerCharacter>(Player);
		AMiniBoss* MiniBoss = Cast<AMiniBoss>(AI->GetPawn());

		LookAt = UKismetMathLibrary::FindLookAtRotation(MiniBoss->GetActorLocation() , MyCharacter->GetActorLocation());
		
		//UE_LOG(LogClass, Warning, TEXT("\nCaculate Rotator pitch : %f \nYaw : %f \n Roll : %f\n"),LookAt.Pitch,LookAt.Yaw,LookAt.Roll);
		if (MiniBoss)
		{
			if (!MiniBoss->CurrentFalling)
			{
				if (Distance > 1000.0f)
				{
					if (MiniBoss->JumpRunCheckFlag)
					{
						MiniBoss->CurrentAnimState = EMiniBossAnimState::JumpAttackReady;
						MiniBoss->JumpRunCheckFlag = false;
					}					
				}
				else if (Distance <1000.0f && Distance >400.0f)
				{
					//UE_LOG(LogClass, Warning, TEXT("대쉬 애드 범위 진입"));
				}
				else if (Distance < 500.0f)
				{
					MiniBoss->WalkStopFlag = true;
					MiniBoss->CurrentState = EMiniBossState::Battle;
					MiniBoss->CurrentAnimState = EMiniBossAnimState::Walk;
				}
			}			
		}
	}
}
