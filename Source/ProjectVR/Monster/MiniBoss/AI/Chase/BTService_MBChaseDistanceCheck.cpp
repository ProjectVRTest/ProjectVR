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
		Distance = AI->BBComponent->GetValueAsFloat(DistanceBlackBoardKey);

		AActor* Player = Cast<AActor>(AI->BBComponent->GetValueAsObject(TEXT("Player")));
		AMotionControllerCharacter* MyCharacter = Cast<AMotionControllerCharacter>(Player);
		AMiniBoss* MiniBoss = Cast<AMiniBoss>(AI->GetPawn());

		if (MiniBoss && MyCharacter)
		{
			FVector Velocity = MiniBoss->GetCharacterMovement()->Velocity;
			FRotator MiniBossRotator = MiniBoss->GetActorRotation();

			FVector NormalVector = UKismetMathLibrary::Normal(Velocity);
			FRotator XNormalRotator = UKismetMathLibrary::MakeRotFromX(NormalVector);
			FRotator CompleteRotator = UKismetMathLibrary::NormalizedDeltaRotator(MiniBossRotator, XNormalRotator);
			MiniBoss->Yaw = CompleteRotator.Yaw;

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
						MiniBoss->CurrentAnimState = EMiniBossAnimState::Walk;
					}
					else if (Distance < 500.0f && Distance >300.0f)
					{
						MiniBoss->CurrentAnimState = EMiniBossAnimState::BattleWalk;						
					}
					else if(Distance <300.0f)
					{
						MiniBoss->CurrentAnimState = EMiniBossAnimState::AttackReady;
						MiniBoss->CurrentState = EMiniBossState::Battle;
					}
					break;
				case EMiniBossAnimState::JumpAttack:
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
