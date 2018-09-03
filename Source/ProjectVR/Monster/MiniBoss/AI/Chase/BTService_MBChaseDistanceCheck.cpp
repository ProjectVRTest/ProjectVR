// Fill out your copyright notice in the Description page of Project Settings.

#include "BTService_MBChaseDistanceCheck.h"
#include "Headers/MiniBossAIHeader.h"

void UBTService_MBChaseDistanceCheck::TickNode(UBehaviorTreeComponent & OwnerComp, uint8 * NodeMemory, float DeltaSeconds)
{
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);

	FName DistanceBlackBoardKey = GetSelectedBlackboardKey();

	AMiniBossAIController* AI = Cast<AMiniBossAIController>(OwnerComp.GetAIOwner());

	FVector Velocity;
	FRotator TestRotator;

	if (AI)
	{
		FRotator LookAt;
		Distance = AI->BBComponent->GetValueAsFloat(DistanceBlackBoardKey);

		AActor* Player = Cast<AActor>(AI->BBComponent->GetValueAsObject(TEXT("Player")));
		AMotionControllerCharacter* MyCharacter = Cast<AMotionControllerCharacter>(Player);
		AMiniBoss* MiniBoss = Cast<AMiniBoss>(AI->GetPawn());

		LookAt = UKismetMathLibrary::FindLookAtRotation(MiniBoss->GetActorLocation() , MyCharacter->GetActorLocation());

		if (MiniBoss && Player)
		{
			Velocity = MiniBoss->GetCharacterMovement()->Velocity;
			TestRotator = MiniBoss->GetActorRotation();

			FVector NormalVector = UKismetMathLibrary::Normal(Velocity);
			FRotator XNormalRotator = UKismetMathLibrary::MakeRotFromX(NormalVector);
			FRotator CompleteRotator = UKismetMathLibrary::NormalizedDeltaRotator(TestRotator, XNormalRotator);
			MiniBoss->Yaw = CompleteRotator.Yaw;

			//GLog->Log(FString::Printf(TEXT("Yaw : %0.1f"), MiniBoss->Yaw));

			if (!MiniBoss->CurrentFalling)
			{
				switch (MiniBoss->CurrentAnimState)
				{
				case EMiniBossAnimState::Walk:
					if (Distance > 1300.0f)
					{
						MiniBoss->CurrentAnimState = EMiniBossAnimState::JumpAttackReady; 											
					}
					else if (Distance <1000.0f && Distance >500.0f)
					{
						//GLog->Log(FString::Printf(TEXT("대쉬 애드 범위 진입")));
					}
					else if (Distance < 500.0f && Distance >300.0f)
					{
						//MiniBoss->WalkStopFlag = true;
						AI->BBComponent->SetValueAsInt(TEXT("DashCount"), 0);
						MiniBoss->CurrentAnimState = EMiniBossAnimState::BattleWalk;						
					}
					else if(Distance <300.0f)
					{
						MiniBoss->CurrentShortAttackState = EMiniBossShortAttackState::ShortAttackReady;
						MiniBoss->CurrentAnimState = EMiniBossAnimState::Attack;
						MiniBoss->CurrentState = EMiniBossState::Battle;
					}
					break;
				case EMiniBossAnimState::JumpAttack:
					break;
				case EMiniBossAnimState::DashAttack:
					break;
				case EMiniBossAnimState::BattleWalk:
					if (Distance > 600.0f)
					{
						MiniBoss->CurrentAnimState = EMiniBossAnimState::Walk;
					}
					else if (Distance < 300.0f)
					{
						MiniBoss->CurrentAnimState = EMiniBossAnimState::AttackReady;
						MiniBoss->CurrentState = EMiniBossState::Battle;							
					}
					break;
				}				
			}			
		}
	}
}
