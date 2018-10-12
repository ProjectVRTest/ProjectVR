// Fill out your copyright notice in the Description page of Project Settings.

#include "BTService_NMBattleDistanceCheck.h"
#include "Headers/NormalMonsterAIHeader.h"
void UBTService_NMBattleDistanceCheck::TickNode(UBehaviorTreeComponent & OwnerComp, uint8 * NodeMemory, float DeltaSeconds)
{
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);

	FName DistanceBlackBoardKey = GetSelectedBlackboardKey();

	ANormalMonsterAIController* AI = Cast<ANormalMonsterAIController>(OwnerComp.GetAIOwner());

	if (AI)
	{
		Distance = AI->BBComponent->GetValueAsFloat(DistanceBlackBoardKey);

		AActor* Player = Cast<AActor>(AI->BBComponent->GetValueAsObject(TEXT("Player")));

		if (Player)
		{
			AMotionControllerCharacter* MyCharacter = Cast<AMotionControllerCharacter>(Player);

			if (MyCharacter)
			{
				ANormalMonster* NormalMonster = Cast<ANormalMonster>(AI->GetPawn());

				if (NormalMonster)
				{
					Velocity = NormalMonster->GetCharacterMovement()->Velocity;
					NMRotator = NormalMonster->GetActorRotation();

					FVector NormalVector = UKismetMathLibrary::Normal(Velocity);
					FRotator XNormalRotator = UKismetMathLibrary::MakeRotFromX(NormalVector);
					FRotator CompleteRotator = UKismetMathLibrary::NormalizedDeltaRotator(NMRotator, XNormalRotator);
					NormalMonster->Yaw = CompleteRotator.Yaw;

					Distance = AI->BBComponent->GetValueAsFloat(DistanceBlackBoardKey);

					if (Distance > 400.0f )
					{
						switch (NormalMonster->CurrentAnimState)
						{
						case ENormalMonsterAnimState::Walk:
							NormalMonster->CurrentAnimState = ENormalMonsterAnimState::Walk;
							NormalMonster->CurrentState = ENormalMonsterState::Chase;
							break;
						}						
					}
					else
					{
						switch (NormalMonster->CurrentAnimState)
						{
						case ENormalMonsterAnimState::Walk:
							if (Distance < 300.0f)
							{
								NormalMonster->CurrentAnimState = ENormalMonsterAnimState::AttackReady;
							}
							break;
						}
					}				
				}
			}
		}
	}
}
