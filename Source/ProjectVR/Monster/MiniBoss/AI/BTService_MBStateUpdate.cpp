// Fill out your copyright notice in the Description page of Project Settings.

#include "BTService_MBStateUpdate.h"
#include "Headers/MiniBossAIHeader.h"
#include "MyCharacter/CameraLocation.h"

void UBTService_MBStateUpdate::TickNode(UBehaviorTreeComponent & OwnerComp, uint8 * NodeMemory, float DeltaSeconds)
{
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);

	AMiniBossAIController* AI = Cast<AMiniBossAIController>(OwnerComp.GetAIOwner());

	if (AI)
	{
		AMiniBoss* MiniBoss = Cast<AMiniBoss>(AI->GetPawn());

		AActor* Player = Cast<AActor>(AI->BBComponent->GetValueAsObject(TEXT("Player")));

		if (MiniBoss && Player)
		{
			AMotionControllerCharacter* MyCharacter = Cast<AMotionControllerCharacter>(Player);

			if (MyCharacter)
			{
				Distance = FVector::Distance(MiniBoss->GetActorLocation(), MyCharacter->CameraLocation->GetActorLocation());
				AI->BBComponent->SetValueAsFloat("Distance", Distance);
				LookAt = UKismetMathLibrary::FindLookAtRotation(MiniBoss->GetActorLocation(), MyCharacter->CameraLocation->GetActorLocation());
				AI->BBComponent->SetValueAsRotator("LookAtRotator", LookAt);

				switch (MiniBoss->CurrentState)
				{
				case EMiniBossState::Chase:
				{
					FVector Veloctiy = MiniBoss->GetCharacterMovement()->Velocity;
					FRotator MiniBossRotator = MiniBoss->GetActorRotation();

					FVector NormalVector = UKismetMathLibrary::Normal(Veloctiy);
					FRotator XNormalRotator = UKismetMathLibrary::MakeRotFromX(NormalVector);
					FRotator ToCharacterRotator = UKismetMathLibrary::NormalizedDeltaRotator(MiniBossRotator, XNormalRotator);

					MiniBoss->Yaw = ToCharacterRotator.Yaw;

					if (!MiniBoss->CurrentFalling) //중간보스가 점프상태가 아니면
					{
						switch (MiniBoss->CurrentAnimState)
						{
						case EMiniBossAnimState::Walk:
							if (Distance > 1300.0f)
							{
								MiniBoss->CurrentAnimState = EMiniBossAnimState::JumpAttackReady;
							}
							else if (Distance < 1000.0f && Distance > 500.0f)
							{
								MiniBoss->CurrentAnimState = EMiniBossAnimState::Walk;
							}
							else if (Distance <500.0f && Distance >300.0f)
							{
								MiniBoss->CurrentAnimState = EMiniBossAnimState::BattleWalk;
							}
							else if (Distance < 300.0f)
							{
								MiniBoss->CurrentAnimState = EMiniBossAnimState::AttackReady;
								MiniBoss->CurrentState = EMiniBossState::Battle;
							}
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
				break;
				case EMiniBossState::Battle:

					break;
				}
			}
		}
	}
}
