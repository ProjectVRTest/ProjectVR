// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "DontMoveArcherState.generated.h"

UENUM(BlueprintType)
enum class EDontMoveArcherState : uint8
{
	Idle = 0 UMETA(DisplayName = "Idle"),
	Battle = 1 UMETA(DisplayName = "Battle"),
	Dead = 2 UMETA(DisplayName = "Dead")
};

UENUM(BlueprintType)
enum class EDontMoveArcherAnimState : uint8
{
	Wait = 0 UMETA(DisplayName = "Wait"),
	AttackReady = 1 UMETA(DisplayName = "AttackReady"),
	Attack = 2 UMETA(DisplayName = "Attack")
};

UENUM(BlueprintType)
enum class EDontMoveArcherAttackState : uint8
{
	Idle = 0 UMETA(DisplayName = "Idle"),
	BowAttack = 1 UMETA(DisplayName = "BowAttack")
};

UENUM(BlueprintType)
enum class EDontMoveArcherArcherAttackState : uint8
{
	Idle = 0 UMETA(DisplayName = "Idle"),
	ArcherAttackReady = 1 UMETA(DisplayName = "ArcherAttackReady"),
	ArcherAttackStart = 2 UMETA(DisplayName = "ArcherAttackStart")
};