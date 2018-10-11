// Fill out your copyright notice in the Description page of Project Settings.

#include "BTTask_NMStabAttackReadyState.h"
#include "Headers/NormalMonsterAIHeader.h"

EBTNodeResult::Type UBTTask_NMStabAttackReadyState::ExecuteTask(UBehaviorTreeComponent & OwnerComp, uint8 * NodeMemory)
{
	ANormalMonsterAIController* AI = Cast<ANormalMonsterAIController>(OwnerComp.GetAIOwner());

	if (AI)
	{
		AI->BBComponent->SetValueAsFloat("AttackAnimationWaitTime", 0.1f);

		ANormalMonster* NormalMonster = Cast<ANormalMonster>(AI->GetPawn());

		if (NormalMonster)
		{
			float Distance = AI->BBComponent->GetValueAsFloat("Distance");

			if (Distance < 200.0f) //거리가 2M 안이면
			{
				//일반 공격 2콤보 애니메이션을 출력해주기 위해
				AI->BBComponent->SetValueAsFloat("AttackAnimationWaitTime", 1.3f); //셋팅해주고
				AI->BBComponent->SetValueAsFloat("ReverseAnimationWaitTime", 0);  //1공격 회복애니메이션은 필요 없으니까 0으로 설정해준다.
				NormalMonster->CurrentStabAttackState = ENormalMonsterStabAttackState::ClipplingAttackReady; //2번째 콤보 공격 애니메이션을 출력해주기 위해 상태값을 바꿔준다.
			}
			else //거리가 2M 밖이면
			{
				AI->BBComponent->SetValueAsFloat("AttackAnimationWaitTime", 0.05f); //남은 애니메이션을 출력해주기 위해 0.1f로 셋팅해 주고				
				NormalMonster->CurrentStabAttackState = ENormalMonsterStabAttackState::StabAttackStart;	
			}
		}		
	}

	return EBTNodeResult::Succeeded;
}
