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
			switch (MiniBoss->CurrentState)
			{
			case EMiniBossState::Idle:
				break;
			case EMiniBossState::Chase:
				switch (MiniBoss->CurrentAnimState)
				{
				case EMiniBossAnimState::Wait:
					break;
				case EMiniBossAnimState::Walk:
					MiniBoss->GetCharacterMovement()->MaxWalkSpeed = 250.0f;
					break;
				case EMiniBossAnimState::Attack:
					break;
				}
				break;
			case EMiniBossState::Battle:
				switch (MiniBoss->CurrentAnimState)
				{
				case EMiniBossAnimState::Wait:
					break;
				case EMiniBossAnimState::Walk:
					MiniBoss->GetCharacterMovement()->MaxWalkSpeed = 250.0f;
					break;
				case EMiniBossAnimState::Attack:
					break;
				}
				break;
			}			

			AMotionControllerCharacter* MyCharacter = Cast<AMotionControllerCharacter>(Player);

			if (MyCharacter)
			{
				Distance = FVector::Distance(MiniBoss->GetActorLocation(), MyCharacter->CameraLocation->GetActorLocation());
				AI->BBComponent->SetValueAsFloat("Distance", Distance);
			}
		}
	}
}
