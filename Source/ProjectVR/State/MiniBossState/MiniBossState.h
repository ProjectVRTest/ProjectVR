// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "MiniBossState.generated.h"

UENUM(BlueprintType)
enum class EMiniBossState : uint8
{
	Idle = 0 UMETA(DisplayName = "Idle"),
	Patrol = 1 UMETA(DisplayName = "Patrol"),
	Chase = 2 UMETA(DisplayName = "Chase"),
	Battle = 3 UMETA(DisplayName = "Battle"),
	Dead = 4 UMETA(DisplayName = "Dead"),
	Rotate = 5 UMETA(DisplayName = "Rotate")
};

UENUM(BlueprintType)
enum class EMiniBossAnimState : uint8
{
	Wait = 0 UMETA(DisplayName = "Wait"),
	Walk = 1 UMETA(DisplayName = "Walk"),
	JumpAttackReady = 2 UMETA(DisplayName = "JumpAttackReady"),
	JumpAttack = 3 UMETA(DisplayName = "JumpAttack"),
	AttackReady = 4 UMETA(DisplayName = "AttackReady"),
	Attack = 5 UMETA(DisplayName = "Attack"),	
	Parrying = 6 UMETA(DisplayName = "Parrying")
};

UENUM(BlueprintType)
enum class EMiniBossJumpState : uint8
{
	Idle = 0 UMETA(DisplayName = "Idle"),
	JumpStart = 1 UMETA(DisplayName = "JumpStart"),
	JumpLoop = 2 UMETA(DisplayName = "JumpLoop"),
	JumpLand = 3 UMETA(DisplayName = "JumpLand")
};

UENUM(BlueprintType)
enum class EMiniBossAttackState : uint8
{
	Idle = 0 UMETA(DisplayName = "Idle"),
	DefaultAttack = 1 UMETA(DisplayName = "DefaultAttack"),
	ParryingAttack = 2 UMETA(DisplayName = "ParryingAttack")
};