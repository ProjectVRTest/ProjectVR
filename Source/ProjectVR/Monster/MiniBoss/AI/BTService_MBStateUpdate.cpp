// Fill out your copyright notice in the Description page of Project Settings.

#include "BTService_MBStateUpdate.h"
#include "Headers/MiniBossAIHeader.h"

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
			switch (MiniBoss->CurrentAnimState)
			{
			case EMiniBossAnimState::Wait:	
				break;
			case EMiniBossAnimState::Walk:
				MiniBoss->GetCharacterMovement()->MaxWalkSpeed = 200.0f;
				break;
			case EMiniBossAnimState::Attack:
				break;
			}

			AMotionControllerCharacter* MyCharacter = Cast<AMotionControllerCharacter>(Player);

			if (MyCharacter)
			{
				Distance = FVector::Distance(MiniBoss->GetActorLocation(), MyCharacter->Camera->GetComponentLocation());
				AI->BBComponent->SetValueAsFloat("Distance", Distance);
			}
		}
	}
}
