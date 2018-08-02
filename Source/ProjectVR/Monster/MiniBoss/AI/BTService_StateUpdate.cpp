// Fill out your copyright notice in the Description page of Project Settings.

#include "BTService_StateUpdate.h"
#include "Monster/MiniBoss/MiniBossAIController.h"
#include "Monster/MiniBoss/MiniBoss.h"

#include "BehaviorTree/BlackboardComponent.h"

#include "GameFramework/CharacterMovementComponent.h"

#include "MyCharacter/MotionControllerCharacter.h"
#include "Camera/CameraComponent.h"

void UBTService_StateUpdate::InitializeFromAsset(UBehaviorTree & Asset)
{
	Super::InitializeFromAsset(Asset);

	AttackDistance = 280.0f;
}

void UBTService_StateUpdate::TickNode(UBehaviorTreeComponent & OwnerComp, uint8 * NodeMemory, float DeltaSeconds)
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
			Distance = FVector::Distance(MiniBoss->GetActorLocation(), MyCharacter->Camera->GetComponentLocation());
			AI->BBComponent->SetValueAsFloat("Distance", Distance);

			if (MiniBoss->CurrentFalling == false && MiniBoss->IsParrying == false)
			{
				switch (MiniBoss->CurrentState)
				{
				case EMonsterState::Dead:
					MiniBoss->GetCharacterMovement()->MaxWalkSpeed = 0;
					break;
				case EMonsterState::Idle:
					MiniBoss->GetCharacterMovement()->MaxWalkSpeed = 250.0f;
					break;
				case EMonsterState::Chase:
					if (Distance > 1500.0f)
					{
						if (MiniBoss->JumpRunCheckFlag)
						{
							MiniBoss->CurrentAnimState = EMonsterAnimState::JumpAttackReady;
							MiniBoss->JumpRunCheckFlag = false;
						}
					}
					else if (Distance <= 1200.0f)
					{
						if (MiniBoss->CurrentAnimState == EMonsterAnimState::JumpAttack ) //캐릭터가 점프 중인 상태인데
						{
							if (MiniBoss->JumpEndFlag == true) //점프가 끝난 상태가 되면
							{
								MiniBoss->JumpEndFlag = false; //원상태로 돌려주고
								MiniBoss->CurrentAnimState = EMonsterAnimState::Run;
							}
						}
						else
						{
							MiniBoss->CurrentAnimState = EMonsterAnimState::Run;

							if (Distance <= 250.0f)
							{
								MiniBoss->CurrentAnimState = EMonsterAnimState::Walk;
								 
								if (Distance < AttackDistance)
								{
									MiniBoss->CurrentAnimState = EMonsterAnimState::AttackReady;
									MiniBoss->CurrentState = EMonsterState::Battle;
								}
							}
						}
					}

					switch (MiniBoss->CurrentAnimState)
					{
					case EMonsterAnimState::Wait:
						MiniBoss->GetCharacterMovement()->MaxWalkSpeed = 100;
						break;
					case EMonsterAnimState::Walk:
						MiniBoss->GetCharacterMovement()->MaxWalkSpeed = 250.0f;
						break;
					case EMonsterAnimState::Run:
						MiniBoss->GetCharacterMovement()->MaxWalkSpeed = 500.0f;
						break;
					case EMonsterAnimState::JumpAttack:
						MiniBoss->GetCharacterMovement()->MaxWalkSpeed = 100;
						break;
					case EMonsterAnimState::Roar:
						MiniBoss->GetCharacterMovement()->MaxWalkSpeed = 100;
						break;
					case EMonsterAnimState::Parrying:
						MiniBoss->GetCharacterMovement()->MaxWalkSpeed = 100;
						break;
					}
					break;
				case EMonsterState::Battle: 
					if (Distance > 300.0f)
					{
						MiniBoss->CurrentState = EMonsterState::Chase;
						MiniBoss->CurrentAnimState = EMonsterAnimState::Walk;
					}
					break;
				case EMonsterState::Rotate:
					break;
				}
			}
		}
	}
}
