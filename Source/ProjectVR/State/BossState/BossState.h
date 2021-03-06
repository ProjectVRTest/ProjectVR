﻿// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BossState.generated.h"

UENUM(BlueprintType)
enum class EBossState : uint8
{
	Idle = 0 UMETA(DisplayName = "Idle"),	
	Battle = 1 UMETA(DisplayName = "Battle"),
	Groggy = 2 UMETA(DisplayName = "Groggy"),
	Dead = 3 UMETA(DisplayName = "Dead")
};

UENUM(BlueprintType)
enum class EBossBattleState : uint8
{
	Idle = 0 UMETA(DisplayName = "Idle"),
	AttackReady = 1 UMETA(DisplayName = "AttackReady"),
	AddAttack = 2 UMETA(DisplayName = "AddAttack"),
	CloseAttack = 3 UMETA(DisplayName = "CloseAttack"),
	LongAttack = 4 UMETA(DisplayName = "LongAttack"),
	BattleWatch = 5 UMETA(DisplayName = "BattleWatch"), // 전투 주시 상태
	UltimateAttack = 6 UMETA(DisplayName = "UltimateAttack")
};

UENUM(BlueprintType)
enum class EBossBlinkAttackState : uint8
{
	Idle = 0 UMETA(DisplayName = "Idle"),
	AddAttackStart = 1 UMETA(DisplayName ="AddAttackStart"),
	InVisible = 2 UMETA(DisplayName = "InVisible"), //투명화 
	Visible = 3 UMETA(DisplayName = "Visible"), // 플레이어 앞에 나타나면서 불투명화
	BlinkAttack = 4 UMETA(DisplayName = "BlinkAttack") //아래로 내려 찍기
};

UENUM(BlueprintType)
enum class EBossLongAttackState : uint8
{
	Idle = 0 UMETA(DisplayName = "Idle"),
	LongAttackReady = 1 UMETA(DisplayName= "LongAttackReady"),
	DefaultLongAttack = 2 UMETA(DisplayName = "DefaultLongAttack"),
	OrbCreate = 3 UMETA(DisplayName = "OrbCreate"),
	WaveAttack = 4 UMETA(DisplayName = "WaveAttack")
};


UENUM(BlueprintType)
enum class EBossWaveAttackState : uint8
{
	Idle=0 UMETA(DisplayName ="Idle"),
	WaveAttackStart = 1 UMETA(DisplayName = "WaveAttackStart"),
	LeftStep = 2 UMETA(DisplayName = "LeftStep"),
	RightStep = 3 UMETA(DisplayName = "RightStep")
};

UENUM(BlueprintType)
enum class EBossCloseAttackState : uint8
{
	Idle = 0 UMETA(DisplayName = "Idle"),
	CloseAttackReady = 1 UMETA(DisplayName = "CloseAttackReady"),
	RightUpLeftDownAttack = 2 UMETA(DisplayName = "RightUpLeftDownAttack"),
	ComboAttack = 3 UMETA(DisplayName = "ComboAttack"),
	UpDownAttack = 4 UMETA(DisplayName = "UpDownAttack"),
	ParryingState = 5 UMETA(DisplayName = "ParryingState")
};

UENUM(BlueprintType)
enum class EBossUpDownAttackState : uint8
{
	Idle = 0 UMETA(DisplayName = "Idle"),
	UpDownAttackStart = 1 UMETA(DisplayName = "UpDownAttackStart"),
	BackStep = 2 UMETA(DisplayName = "BackStep")
};

UENUM(BlueprintType)
enum class EBossParryingState : uint8
{
	Idle = 0 UMETA(DisplayName = "Idle"),
	ParryingStart = 1 UMETA(DisplayName = "ParryingStart"),
	ParryingLoop = 2 UMETA(DisplayName = "ParryingLoop"),
	ParryingSuccess = 3 UMETA(DisplayName = "ParryingSuccess"),
	CounterAttack = 4 UMETA(DisplayName = "CounterAttack")
};

UENUM(BlueprintType)
enum class EBossBattleWatchState : uint8
{
	Idle = 0 UMETA(DisplayName = "Idle"),
	Walk = 1 UMETA(DisplayName = "Walk"),
	BattleWatchDistanceCheck = 2 UMETA(DisplayName = "BattleWatchDistanceCheck"),
	InVisible = 3 UMETA(DisplayName = "InVisible"),
	Visible = 4 UMETA(DisplayName = "Visible"),
	Confrontation = 5 UMETA(DisplayName = "Confrontation")
};

UENUM(BlueprintType)
enum class EBossConfrontationState : uint8
{
	Idle = 0 UMETA(DisplayName ="Idle"),
	ConfrontationReady = 1 UMETA(DisplayName = "ConfrontationReady"),
	LeftWalk = 2 UMETA(DisplayName = "LeftWalk"),
	RightWalk = 3 UMETA(DisplayNAme = "RightWalk")
};

UENUM(BlueprintType)
enum class EBossUltimateAttackState : uint8
{
	Idle = 0 UMETA(DisplayName = "Idle"),
	InVisible = 1 UMETA(DisplayName = "InVisible"),
	Visible = 2 UMETA(DisplayName = "Visible"),
	UltimateAttackReady = 3 UMETA(DisplayName = "UltimateAttackReady"),
	UltimateAttackStart = 4 UMETA(DisplayName = "UltimateAttackStart"),
	UltimateAttackEnd = 5 UMETA(DisplayName = "UltimateAttackEnd")
};