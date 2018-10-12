// Fill out your copyright notice in the Description page of Project Settings.

#include "BTService_NMDMArcherStateUpdate.h"
#include "Headers/DontMoveArcherAIHeader.h"
#include "Camera/CameraComponent.h"

void UBTService_NMDMArcherStateUpdate::TickNode(UBehaviorTreeComponent & OwnerComp, uint8 * NodeMemory, float DeltaSeconds)
{
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);

	ADontMoveArcherAIController* AI = Cast<ADontMoveArcherAIController>(OwnerComp.GetAIOwner());

	if (AI)
	{
		ADontMoveArcher* DontMoveArcher = Cast<ADontMoveArcher>(AI->GetPawn());

		AActor* Player = Cast<AActor>(AI->BBComponent->GetValueAsObject(TEXT("Player")));

		if (DontMoveArcher && Player)
		{
			AMotionControllerCharacter* MyCharacter = Cast<AMotionControllerCharacter>(Player);

			if (MyCharacter)
			{
				Distance = FVector::Distance(DontMoveArcher->GetActorLocation(), MyCharacter->Camera->GetComponentLocation());
				AI->BBComponent->SetValueAsFloat("Distance", Distance);

				if (Distance < 2000.0f)
				{
					DontMoveArcher->CurrentArcherAttackState = EDontMoveArcherArcherAttackState::ArcherAttackReady;
					DontMoveArcher->CurrentAttackState = EDontMoveArcherAttackState::BowAttack;
					DontMoveArcher->CurrentAnimState = EDontMoveArcherAnimState::Attack;
					DontMoveArcher->CurrentState = EDontMoveArcherState::Battle;
				}
				else
				{
					DontMoveArcher->CurrentArcherAttackState = EDontMoveArcherArcherAttackState::Idle;
					DontMoveArcher->CurrentAttackState = EDontMoveArcherAttackState::Idle;
					DontMoveArcher->CurrentAnimState = EDontMoveArcherAnimState::Wait;
					DontMoveArcher->CurrentState = EDontMoveArcherState::Idle;
				}
			}
		}
	}
}
