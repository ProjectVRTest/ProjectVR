﻿// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "NormalMonsterState.generated.h"

UENUM(BlueprintType)
enum class ENormalMonsterKind : uint8
{
	SwordMan = 0 UMETA(DisplayName = "SwordMan"),
	Archer = 1 UMETA(DisplayName = "Archer")
};

UENUM(BlueprintType)
enum class ENormalMonsterState : uint8
{
	Idle = 0 UMETA(DisplayName = "Idle"),
	Patrol = 1 UMETA(DisplayName = "Patrol"),
	Chase = 2 UMETA(DisplayName = "Chase"),
	Battle = 3 UMETA(DisplayName = "Battle"),
	Dead = 4 UMETA(DisplayName = "Dead"),
	Rotate = 5 UMETA(DisplayName = "Rotate")
};

UENUM(BlueprintType)
enum class ENormalMonsterAnimState : uint8
{
	Wait =0 UMETA(DisplayName = "Wait"),
	AttackWait = 1 UMETA(DisplayName = "AttackWait"), //공격을 받을때까지 대기하는 애니메이션 상태
	BattleWait = 2 UMETA(DisplayName = "BattleWait"), //전투공격 대기 상태
	Walk = 3 UMETA(DisplayName = "Walk"),
	Run = 4 UMETA(DisplayName = "Run"),
	RageRun = 5 UMETA(DisplayName = "RageRun"),
	RageAttack = 6 UMETA(DisplayName = "RageAttack"),
	AttackReady = 7 UMETA(DisplayName = "AttackReady"),
	Attack = 8 UMETA(DisplayName = "Attack")
};

UENUM(BlueprintType)
enum class ENormalMonsterIdleState : uint8
{
	Wait = 0 UMETA(DisplayName = "Wait"),
	Sprawling = 1 UMETA(DisplayName = "Sprawling"),
	Bed = 2 UMETA(DisplayName = "Bed"),
	SitDown = 3 UMETA(DisplayName = "SitDown"),
	Hide = 4 UMETA(DisplayName= "Hide")
};

UENUM(BlueprintType)
enum class ENormalMonsterAttackState : uint8
{
	Stand = 0 UMETA(DisplayName = "Stand"),
	StabAttackReady = 1 UMETA(DisplayName = "StabAttackReady"),
	StabAttackStart = 2 UMETA(DisplayName = "StabAttackStart"),
	ClipplingAttackReady = 3 UMETA(DisplayName = "ClipplingAttackReady"),
	ClipplingAttackStart = 4 UMETA(DisplayName = "ClipplingAttackStart"),
	ComboAttackReady = 5 UMETA(DisplayName = "ComboAttack"),
	ComboAttackReadyStart = 6 UMETA(DisplayName = "ComboAttackReadyStart")
};