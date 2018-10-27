// Fill out your copyright notice in the Description page of Project Settings.

#include "BTService_NMChaseDistanceCheck.h"
#include "Headers/NormalMonsterAIHeader.h"


void UBTService_NMChaseDistanceCheck::InitializeFromAsset(UBehaviorTree & Asset)
{
	Super::InitializeFromAsset(Asset);
	ChaseWalkTimeCount = 0;
	ChaseRunTimerCount = 0;
	RageAttackFlag = false;
}

void UBTService_NMChaseDistanceCheck::TickNode(UBehaviorTreeComponent & OwnerComp, uint8 * NodeMemory, float DeltaSeconds)
{
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);

	FName DistanceBlackBoardKey = GetSelectedBlackboardKey();

	ANormalMonsterAIController* AI = Cast<ANormalMonsterAIController>(OwnerComp.GetAIOwner());

	if (AI)
	{
		Distance = AI->BBComponent->GetValueAsFloat(DistanceBlackBoardKey);

		NormalMonster = Cast<ANormalMonster>(AI->GetPawn());

		if (NormalMonster)
		{
			switch(NormalMonster->MonsterKind)
			{
			case ENormalMonsterKind::SwordMan:
				if (Distance < 200.0f)
				{
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
				else
				{
					switch (NormalMonster->CurrentAnimState)
					{
					case ENormalMonsterAnimState::Wait:
						NormalMonster->CurrentAnimState = ENormalMonsterAnimState::Walk;
						break;
					case ENormalMonsterAnimState::Walk: //걷기 상태라면
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
					case ENormalMonsterAnimState::RageAttack:
						if (Distance > 200.0f)
						{
							//GLog->Log(FString::Printf(TEXT("RageAttack Walk")));						
							if (!GetWorld()->GetTimerManager().TimerExists(RageAttackTimer)) //타이머가 현재 활성화 되어 있는지 확인해준다.
							{
								GLog->Log(FString::Printf(TEXT("분노공격 타이머 비활성화")));
								if (!RageAttackFlag)
								{
									GLog->Log(FString::Printf(TEXT("분노공격 타이머 시작")));
									GetWorld()->GetTimerManager().SetTimer(RageAttackTimer, this, &UBTService_NMChaseDistanceCheck::RageAttackTimerCount, 1.7f, false);
								}
							}

							if (RageAttackFlag) //타이머에서 RageAttackFlag가 true가 될때까지 기다려서 true가 되면 Walk상태로 바꿔준다.
							{
								NormalMonster->CurrentAnimState = ENormalMonsterAnimState::Walk;
							}
						}
						else //거리가 만약에 2M보다 작다면
						{
							//공격 상태로 바꿔준다.
							NormalMonster->CurrentAnimState = ENormalMonsterAnimState::AttackReady;
							NormalMonster->CurrentState = ENormalMonsterState::Battle;
						}
					}
				}
				break;
			case ENormalMonsterKind::MoveArcher:
				if (Distance < 1500.0f)
				{
					NormalMonster->CurrentArcherAttackState = ENormalMonsterArcherAttackState::ArcherAttackReady;
					NormalMonster->CurrentAttackState = ENormalMonsterAttackState::BowAttack;
					NormalMonster->CurrentAnimState = ENormalMonsterAnimState::Attack;
					NormalMonster->CurrentState = ENormalMonsterState::Battle;				
				}
				break;
			}		
		}
	}
}

void UBTService_NMChaseDistanceCheck::RageAttackTimerCount()
{
	GLog->Log(FString::Printf(TEXT("분노 공격 타이머 콜")));
	RageAttackFlag = true;
}
