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
			if (Distance < 400.0f)
			{
				//GLog->Log(FString::Printf(TEXT("RageAttackFlag %d"), RageAttackFlag));
				//몬스터의 상태가 RageAttack에 진입할때에는 RageAttackFlag가 true인지 확인해 줘서 전투 상태에 진입시킨다.
							
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
				/*else
				{
					NormalMonster->CurrentState = ENormalMonsterState::Battle;
					NormalMonster->CurrentAnimState = ENormalMonsterAnimState::AttackReady;
				}*/
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
				case ENormalMonsterAnimState::RageRun:
					if (Distance < 500.0f)
					{
					//	GLog->Log(FString::Printf(TEXT("Chase RageRun Distance 500.0f 미만")));
						NormalMonster->CurrentAnimState = ENormalMonsterAnimState::RageAttack;
					}					
					break;
				case ENormalMonsterAnimState::RageAttack:
					if (Distance > 400.0f)
					{
						//GLog->Log(FString::Printf(TEXT("RageAttack Walk")));						
						if (!GetWorld()->GetTimerManager().TimerExists(RageAttackTimer)) //타이머가 현재 활성화 되어 있는지 확인해준다.
						{
							//활성화 되어 있지 않다면							
							//분노 추적 공격 타이머를 생성해준다.
							GLog->Log(FString::Printf(TEXT("NoExists")));
							if (!RageAttackFlag)
							{
								GLog->Log(FString::Printf(TEXT("TimerStart")));
								GetWorld()->GetTimerManager().SetTimer(RageAttackTimer, this, &UBTService_NMChaseDistanceCheck::RageAttackTimerCount, 2.0f, false);
							}					
						}					
						
						if (RageAttackFlag) //타이머에서 RageAttackFlag가 true가 될때까지 기다려서 true가 되면 Walk상태로 바꿔준다.
						{
							NormalMonster->CurrentAnimState = ENormalMonsterAnimState::Walk;
						}						
					}
					else //거리가 만약에 4M보다 작다면
					{
						//공격 상태로 바꿔준다.
						NormalMonster->CurrentAnimState = ENormalMonsterAnimState::AttackReady;
						NormalMonster->CurrentState = ENormalMonsterState::Battle;
					}
					//GLog->Log(FString::Printf(TEXT("Chase RageAttack")));
				}
			}
		}
		/*if (NormalMonster)
		{		
			switch (NormalMonster->CurrentAnimState)
			{
			case ENormalMonsterAnimState::Wait:
				if (Distance >= 400.0f)
				{
					NormalMonster->CurrentAnimState = ENormalMonsterAnimState::Walk;
				}
				else if (Distance < 400.0f)
				{
					NormalMonster->CurrentAnimState = ENormalMonsterAnimState::AttackReady;
					NormalMonster->CurrentState = ENormalMonsterState::Battle;
				}
				break;
			case ENormalMonsterAnimState::Walk:
				if (Distance < 400.0f)
				{
					NormalMonster->CurrentAnimState = ENormalMonsterAnimState::AttackReady;
					NormalMonster->CurrentState = ENormalMonsterState::Battle;
				}
				break;
			case ENormalMonsterAnimState::Run:
				if (Distance < 400.0f)
				{
					NormalMonster->CurrentAnimState = ENormalMonsterAnimState::AttackReady;
					NormalMonster->CurrentState = ENormalMonsterState::Battle;
				}
				break;
			case ENormalMonsterAnimState::RageRun:
				if (Distance <= 400.0f)
				{
					NormalMonster->CurrentAnimState = ENormalMonsterAnimState::RageAttack;
				}
				break;
			}
		}*/
	}
}

void UBTService_NMChaseDistanceCheck::RageAttackTimerCount()
{
	GLog->Log(FString::Printf(TEXT("RageAttackTimer Call")));
	RageAttackFlag = true;
}
