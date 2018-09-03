// Fill out your copyright notice in the Description page of Project Settings.

#include "BTTask_MBBackAttackState.h"
#include "Headers/MiniBossAIHeader.h"

EBTNodeResult::Type UBTTask_MBBackAttackState::ExecuteTask(UBehaviorTreeComponent & OwnerComp, uint8 * NodeMemory)
{
	GLog->Log(FString::Printf(TEXT("뒤돌기 공격 상태 진입")));
	return EBTNodeResult::Succeeded;
}
