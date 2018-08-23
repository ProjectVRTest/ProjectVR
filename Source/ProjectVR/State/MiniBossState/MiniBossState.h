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
	Attack = 4 UMETA(DisplayName = "Attack"),	
	Parrying = 5 UMETA(DisplayName = "Parrying"),
	BackWalk = 6 UMETA(DisplayName = "BackWalk"),
	DashAttack = 7 UMETA(DisplayName = "DashAttack")
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
enum class EMiniBossDashState : uint8
{
	Idle = 0 UMETA(DisplayName = "Idle"),
	DashStart = 1 UMETA(DisplayName = "DashStart"),
	DashLoop = 2 UMETA(DisplayName = "DashLoop"),
	DashEnd = 3 UMETA(DisplayName = "DashEnd")
};

UENUM(BlueprintType)
enum class EMiniBossAttackState : uint8
{
	Idle = 0 UMETA(DisplayName = "Idle"),
	AttackReady = 1 UMETA(DisplayName = "AttackReady"),
	RightUpLeftDown = 2 UMETA(DisplayName = "RightUpLeftDown"),
	TwoHandWidth = 3 UMETA(DisplayName = "TwoHandWidth"),
	StabReady = 4 UMETA(DisplayName = "StabReady"),
	LongDistance = 5 UMETA(DisplayName = "LongDistance"),
	FourCombo = 6 UMETA(DisplayName = "FourCombo"),
	BackAttack = 7 UMETA(DisplayName = "BackAttack"),
	StabStart = 8 UMETA(DisplayName = "StabStart")
};