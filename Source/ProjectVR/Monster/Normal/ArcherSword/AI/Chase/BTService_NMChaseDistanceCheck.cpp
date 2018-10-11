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

		ANormalMonster* NormalMonster = Cast<ANormalMonster>(AI->GetPawn());

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
						RageAttackFlag = false;  //분노 공격 상태가 되면 초기값으로 돌려준다.
						break;
					case ENormalMonsterAnimState::Run:
						break;
					case ENormalMonsterAnimState::RageRun: //분노 달리기 상태 일때
						if (Distance < 250.0f) //거리가 2.5m아래이면
						{
							//분노 공격 상태로 바꿔준다.
							NormalMonster->CurrentAnimState = ENormalMonsterAnimState::RageAttack;
						}
						break;
					case ENormalMonsterAnimState::RageAttack: //분노 공격 상태 일때
						if (Distance > 200.0f)
						{
							//GLog->Log(FString::Printf(TEXT("RageAttack Walk")));						
							if (!GetWorld()->GetTimerManager().TimerExists(RageAttackTimer)) //타이머가 현재 활성화 되어 있는지 확인해준다.
							{
								//활성화 되어 있지 않다면							
								//분노 추적 공격 타이머를 생성해준다.
								if (!RageAttackFlag)
								{
									//1.7초 후에 분노공격 타이머를 생성해서 분노 공격 애니메이션이 나올수 있게 해준다.
									GetWorld()->GetTimerManager().SetTimer(RageAttackTimer, this, &UBTService_NMChaseDistanceCheck::RageAttackTimerCount, 1.7f, false);
								}
							}

							//RageAttackTimerCount가 발동되면 RageAttackFlag가 true가 되어서 
							if (RageAttackFlag) 
							{
								//Walk로 바꿔서 다시 거리를 판정할 수 있게 해준다.
								NormalMonster->CurrentAnimState = ENormalMonsterAnimState::Walk;
							}
						}
						else //거리가 만약에 2m보다 작다면
						{
							//공격 상태로 바꿔준다.
							NormalMonster->CurrentAnimState = ENormalMonsterAnimState::AttackReady;
							NormalMonster->CurrentState = ENormalMonsterState::Battle;
						}
					}
				}
				break;
			case ENormalMonsterKind::MoveArcher: //캐릭터의 종류가 일반몬스터이면
				if (Distance < 1500.0f) //거리가 15m 아래 일때
				{
					//공격한다.
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
	RageAttackFlag = true;
}
