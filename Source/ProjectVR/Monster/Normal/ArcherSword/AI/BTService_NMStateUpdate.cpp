// Fill out your copyright notice in the Description page of Project Settings.

#include "BTService_NMStateUpdate.h"
#include "Headers/NormalMonsterAIHeader.h"

#include "MyCharacter/MotionControllerCharacter.h"
#include "MyCharacter/CameraLocation.h"

void UBTService_NMStateUpdate::InitializeFromAsset(UBehaviorTree & Asset)
{
	Super::InitializeFromAsset(Asset);
	Distance = 0;
	RageAttackFlag = false;
}

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

				if (MyCharacter)
				{
					Distance = FVector::Distance(NormalMonster->GetActorLocation(), MyCharacter->CameraLocation->GetActorLocation());
					AI->BBComponent->SetValueAsFloat("Distance", Distance);

					switch (NormalMonster->MonsterKind)
					{
					case ENormalMonsterKind::SwordMan:
						if (Distance > 800.0f && NormalMonster->ArrowCount > 0) //거리가 8m 보다 크고 가지고 잇는 화살 수가 0보다 크면
						{
							NormalMonster->ChangeFormBow();  //무기를 바꾼다.
						}
						else 
						{
							SwordManLocomotion(NormalMonster);
						}
						break;
					case ENormalMonsterKind::MoveArcher:						
						BowManLocomotion(NormalMonster);
						break;
					}
				}
			}
		}
	}
}

void UBTService_NMStateUpdate::RageAttackTimerCount()
{
	RageAttackFlag = true;
}

void UBTService_NMStateUpdate::SwordManLocomotion(ANormalMonster* NormalMonster)
{
	switch (NormalMonster->CurrentState)
	{
	case ENormalMonsterState::Chase:
		if (Distance < 200.0f) //거리가 2m 보다 작아지면 전투 상태로 바꿔준다.
		{
			//이때 몬스터의 CurrentAnimState가 RageAttack이면 (분노 공격 중이라면)
			//RageAttackFlag가 true가 될때까진 기다려준다.
			if (NormalMonster->CurrentAnimState == ENormalMonsterAnimState::RageAttack)
			{
				if (RageAttackFlag)
				{
					NormalMonster->CurrentState = ENormalMonsterState::Battle;
					NormalMonster->CurrentAnimState = ENormalMonsterAnimState::AttackReady;
				}
			}
			else
			{
				NormalMonster->CurrentState = ENormalMonsterState::Battle;
				NormalMonster->CurrentAnimState = ENormalMonsterAnimState::AttackReady;
			}
		}
		else //거리가 2m 보다 크다면
		{
			switch (NormalMonster->CurrentAnimState)
			{
			case ENormalMonsterAnimState::Wait:
				NormalMonster->CurrentAnimState = ENormalMonsterAnimState::Walk;
				break;
			case ENormalMonsterAnimState::Walk:
				RageAttackFlag = false;
				break;
			case ENormalMonsterAnimState::Run:
				break;
			case ENormalMonsterAnimState::RageRun:
				if (Distance < 250.0f)
				{
					NormalMonster->CurrentAnimState = ENormalMonsterAnimState::RageAttack;
				}
				break;
			case ENormalMonsterAnimState::RageAttack: //분노공격일때
				if (Distance > 200.0f) //거리가 2M 보다 크다면
				{
					if (!GetWorld()->GetTimerManager().TimerExists(RageAttackTimer))
					{
						if (!RageAttackFlag)
						{
							//GLog->Log(FString::Printf(TEXT("분노공격 타이머 시작1")));
							GetWorld()->GetTimerManager().SetTimer(RageAttackTimer, this, &UBTService_NMStateUpdate::RageAttackTimerCount, 1.7f, false);
						}
					}

					if (RageAttackFlag)
					{
						NormalMonster->CurrentAnimState = ENormalMonsterAnimState::Walk;
					}
				}
				else
				{
					NormalMonster->CurrentAnimState = ENormalMonsterAnimState::AttackReady;
					NormalMonster->CurrentState = ENormalMonsterState::Battle;
				}
				break;
			}
		}
		break;
	case ENormalMonsterState::Battle:
		if (NormalMonster->CurrentAnimState == ENormalMonsterAnimState::Walk)
		{
			if (Distance > 400.0f)
			{
				NormalMonster->CurrentAnimState = ENormalMonsterAnimState::Walk;
				NormalMonster->CurrentState = ENormalMonsterState::Chase;
			}
			else
			{
				NormalMonster->CurrentAnimState = ENormalMonsterAnimState::AttackReady;
			}
		}
		break;
	}
}

void UBTService_NMStateUpdate::BowManLocomotion(ANormalMonster * NormalMonster)
{
	switch (NormalMonster->CurrentState)
	{
	case ENormalMonsterState::Chase:
		if (Distance < 1500.0f)
		{
			NormalMonster->CurrentArcherAttackState = ENormalMonsterArcherAttackState::ArcherAttackReady;
			NormalMonster->CurrentAttackState = ENormalMonsterAttackState::BowAttack;
			NormalMonster->CurrentAnimState = ENormalMonsterAnimState::Attack;
			NormalMonster->CurrentState = ENormalMonsterState::Battle;

			if (Distance < 500.0f)
			{
				NormalMonster->ChangeFormSword();
			}
		}
		break;
	case ENormalMonsterState::Battle:
		if (Distance > 2500.0f)
		{
			NormalMonster->CurrentArcherAttackState = ENormalMonsterArcherAttackState::idle;
			NormalMonster->CurrentAttackState = ENormalMonsterAttackState::Idle;
			NormalMonster->CurrentAnimState = ENormalMonsterAnimState::Walk;
			NormalMonster->CurrentState = ENormalMonsterState::Chase;
		}
		else if (Distance < 500.0f)
		{
			NormalMonster->ChangeFormSword();
		}
		break;
	}
}
