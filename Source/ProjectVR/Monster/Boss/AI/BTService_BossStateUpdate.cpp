// Fill out your copyright notice in the Description page of Project Settings.

#include "BTService_BossStateUpdate.h"
#include "Headers/BossAIHeader.h"
#include "MyCharacter/CameraLocation.h"

void UBTService_BossStateUpdate::TickNode(UBehaviorTreeComponent & OwnerComp, uint8 * NodeMemory, float DeltaSeconds)
{
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);

	ABossAIController* AI = Cast<ABossAIController>(OwnerComp.GetAIOwner());

	if (AI)
	{
		ABoss* Boss = Cast<ABoss>(AI->GetPawn());

		AActor* Player = Cast<AActor>(AI->BBComponent->GetValueAsObject(TEXT("Player")));

		if (Boss && Player)
		{
			switch (Boss->CurrentState)
			{
			case EBossState::Idle:
				break;
			case EBossState::Battle:
				break;
			case EBossState::Groggy:
				break;
			case EBossState::Dead:
				break;
			}

			AMotionControllerCharacter* MyCharacter = Cast<AMotionControllerCharacter>(Player);

			if (MyCharacter)
			{
				Distance = FVector::Distance(Boss->GetActorLocation(), MyCharacter->CameraLocation->GetActorLocation());
				AI->BBComponent->SetValueAsFloat("Distance", Distance);
			}
		}
	}
}
