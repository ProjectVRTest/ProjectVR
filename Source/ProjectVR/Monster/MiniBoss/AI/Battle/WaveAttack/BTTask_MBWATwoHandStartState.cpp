﻿// Fill out your copyright notice in the Description page of Project Settings.

#include "BTTask_MBWATwoHandStartState.h"
#include "Headers/MiniBossAIHeader.h"

EBTNodeResult::Type UBTTask_MBWATwoHandStartState::ExecuteTask(UBehaviorTreeComponent & OwnerComp, uint8 * NodeMemory)
{
	GLog->Log(FString::Printf(TEXT("공격 패턴 2 가로베기 준비 상태 진입")));
	return EBTNodeResult::Succeeded;
}