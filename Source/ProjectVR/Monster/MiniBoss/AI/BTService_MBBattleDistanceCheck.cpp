// Fill out your copyright notice in the Description page of Project Settings.

#include "BTService_MBBattleDistanceCheck.h"
#include "Headers/MiniBossAIHeader.h"

void UBTService_MBBattleDistanceCheck::TickNode(UBehaviorTreeComponent & OwnerComp, uint8 * NodeMemory, float DeltaSeconds)
{
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);

	FName DistanceBlackBoardKey = GetSelectedBlackboardKey();
	//GLog->Log(FString::Printf(TEXT("if -1f")));
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
				if (MiniBoss)
				{
					/*LookAt = UKismetMathLibrary::FindLookAtRotation(MiniBoss->GetActorLocation(), MyCharacter->GetActorLocation());
					CurrentRot = FMath::Lerp(MiniBoss->GetActorRotation(), LookAt, DeltaSeconds);

					MiniBoss->SetActorRotation(CurrentRot);*/

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
							if (Distance > 500.0f)
							{
								MiniBoss->WalkStopFlag = false;
								MiniBoss->IsAttack = false; //다시 공격할 수 있게 해줌
								MiniBoss->CurrentAnimState = EMiniBossAnimState::Walk;
								MiniBoss->CurrentState = EMiniBossState::Chase;								
							}
							else if (Distance < 300.0f && !MiniBoss->IsAttack)
							{
								//GLog->Log(FString::Printf(TEXT("Walk -> Attack")));
								MiniBoss->CurrentAttackState = EMiniBossAttackState::AttackReady;
								MiniBoss->CurrentAnimState = EMiniBossAnimState::Attack;								
								MiniBoss->IsAttack = true; //공격중이란것을 나타냄
							}
							break;
						case EMiniBossAnimState::Attack:
							switch (MiniBoss->CurrentAttackState)
							{
							case EMiniBossAttackState::AttackReady:
								if (Distance > 500.0f && MiniBoss->AttackCompleteFlag)
								{
									MiniBoss->AttackCompleteFlag = false;
									MiniBoss->IsAttack = false; //다시 공격할 수 있게 해줌
									MiniBoss->CurrentAnimState = EMiniBossAnimState::Walk;
									MiniBoss->CurrentAttackState = EMiniBossAttackState::AttackReady;
									MiniBoss->CurrentState = EMiniBossState::Chase;									
								}
								break;
							case EMiniBossAttackState::RightUpLeftDownReady:								
								break;
							case EMiniBossAttackState::RightUpLeftDownStart:
								break;
							case EMiniBossAttackState::StabReady:
								break;
							case EMiniBossAttackState::StabStart:
								break;
							case EMiniBossAttackState::TwoHandWidthReady:
								break;
							case EMiniBossAttackState::TwoHandWidthStart:
								break;
							}							
							break;
						case EMiniBossAnimState::BackWalk:
							//GLog->Log(FString::Printf(TEXT("BackWalk")));
							break;
						}
					}						
				}
			}
		}	
	}
}
