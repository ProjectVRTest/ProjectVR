// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

UENUM(BlueprintType)
enum class EMonsterState : uint8
{
	Idle = 0 UMETA(DisplayName = "Idle"),
	Patrol = 1 UMETA(DisplayName = "Patrol"),
	Chase = 2 UMETA(DisplayName = "Chase"),
	Battle = 3 UMETA(DisplayName = "Battle"),
	Dead = 4 UMETA(DisplayName = "Dead"),
	Rotate = 5 UMETA(DisplayName = "Rotate")
};

UENUM(BlueprintType)
enum class EMonsterAnimState : uint8
{
	Wait = 0 UMETA(DisplayName = "Wait"),
	Walk = 1 UMETA(DisplayName = "Walk"),
	Run = 2 UMETA(DisplayName = "Run"),
	JumpAttackReady = 3 UMETA(DisplayName = "JumpAttackReady"),
	JumpAttack = 4 UMETA(DisplayName = "JumpAttack"),
	AttackReady = 5 UMETA(DisplayName = "AttackReady"),
	Attack = 6 UMETA(DisplayName = "Attack"),
	Roar = 7 UMETA(DisplayName = "Roar"),
	Parrying = 8 UMETA(DisplayName = "Parrying")
};

UENUM(BlueprintType)
enum class EMonsterJumpState : uint8
{
	Idle = 0 UMETA(DisplayName = "Idle"),
	JumpStart = 1 UMETA(DisplayName = "JumpStart"),
	JumpLoop = 2 UMETA(DisplayName = "JumpLoop"),
	JumpLand = 3 UMETA(DisplayName = "JumpLand")
};

UENUM(BlueprintType)
enum class EMonsterAttackState : uint8
{
	Idle = 0 UMETA(DisplayName = "Idle"),
	DefaultAttack = 1 UMETA(DisplayName = "DefaultAttack"),
	ParryingAttack = 2 UMETA(DisplayName = "ParryingAttack")
};
