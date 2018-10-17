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
	LongAttack = 4 UMETA(DisplayName = "LongAttack")	
};

UENUM(BlueprintType)
enum class EBossBlinkAttackState : uint8
{
	Idle = 0 UMETA(DisplayName = "Idle"),
	AddAttackStart = 1 UMETA(DisplayName ="AddAttackStart"),
	InVisible = 2 UMETA(DisplayName = "InVisible"), //투명화 
	Visible = 3 UMETA(DisplayName = "Visible"), // 플레이어 앞에 나타나면서 불투명화
	BlinkAttack = 4 UMETA(DisplayName = "BlinkAttack")
};

UENUM(BlueprintType)
enum class EBossLongAttackState : uint8
{
	Idle = 0 UMETA(DisplayName = "Idle"),
	LongAttackReady = 1 UMETA(DisplayName= "LongAttackReady"),
	DefaultLongAttack = 2 UMETA(DisplayName = "DefaultLongAttack"),
	OrbCreate = 3 UMETA(DisplayName = "OrbCreate")
};
