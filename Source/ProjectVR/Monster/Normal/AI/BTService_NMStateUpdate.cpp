// Fill out your copyright notice in the Description page of Project Settings.

#include "BTService_NMStateUpdate.h"
#include "Headers/NormalMonsterAIHeader.h"

#include "MyCharacter/MotionControllerCharacter.h"
#include "Camera/CameraComponent.h"

void UBTService_NMStateUpdate::TickNode(UBehaviorTreeComponent & OwnerComp, uint8 * NodeMemory, float DeltaSeconds)
{
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);

	ANormalMonsterAIController* AI = Cast<ANormalMonsterAIController>(OwnerComp.GetAIOwner());

	if (AI)
	{
		ANormalMonster* NormalMonster = Cast<ANormalMonster>(AI->GetPawn());

		AActor* Player = Cast<AActor>(AI->BBComponent->GetValueAsObject(TEXT("Player")));

		if (NormalMonster)
		{
			switch (NormalMonster->CurrentAnimState)
			{
			case ENormalMonsterAnimState::Wait:
				NormalMonster->GetCharacterMovement()->MaxWalkSpeed = 0;
				break;
			case ENormalMonsterAnimState::Walk:
				NormalMonster->GetCharacterMovement()->MaxWalkSpeed = 250.0f;
				break;
			case ENormalMonsterAnimState::Run:
				NormalMonster->GetCharacterMovement()->MaxWalkSpeed = 350.0f;
				break;
			case ENormalMonsterAnimState::RageRun:
				NormalMonster->GetCharacterMovement()->MaxWalkSpeed = 450.0f;
				break;
			case ENormalMonsterAnimState::RageAttack:
				break;
			}

			if (Player)
			{
				AMotionControllerCharacter* MyCharacter = Cast<AMotionControllerCharacter>(Player);
				Distance = FVector::Distance(NormalMonster->GetActorLocation(), MyCharacter->Camera->GetComponentLocation());
				AI->BBComponent->SetValueAsFloat("Distance", Distance);
			}
		}
	}
}
