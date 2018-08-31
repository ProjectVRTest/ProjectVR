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
	Groggy = 5 UMETA(DisplayName = "Groggy")
};

UENUM(BlueprintType)
enum class EMiniBossAnimState : uint8
{
	Wait = 0 UMETA(DisplayName = "Wait"),
	Walk = 1 UMETA(DisplayName = "Walk"),
	JumpAttackReady = 2 UMETA(DisplayName = "JumpAttackReady"),
	JumpAttack = 3 UMETA(DisplayName = "JumpAttack"),
	Attack = 4 UMETA(DisplayName = "Attack"),	
	ParryingReady = 5 UMETA(DisplayName = "ParryingReady"),
	BackWalk = 6 UMETA(DisplayName = "BackWalk"),
	DashAttack = 7 UMETA(DisplayName = "DashAttack"),
	Rotate = 6 UMETA(DisplayNAme = "Rotate")
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
	RightUpLeftDownReady = 2 UMETA(DisplayName = "RightUpLeftDownReady"),
	TwoHandWidthReady = 3 UMETA(DisplayName = "TwoHandWidthReady"),
	StabReady = 4 UMETA(DisplayName = "StabReady"),
	LongDistance = 5 UMETA(DisplayName = "LongDistance"),
	FourCombo = 6 UMETA(DisplayName = "FourCombo"),
	BackAttack = 7 UMETA(DisplayName = "BackAttack"),
	StabStart = 8 UMETA(DisplayName = "StabStart"),
	RightUpLeftDownStart = 9 UMETA(DisplayName = "RightUpLeftDownStart"),
	TwoHandWidthStart = 10 UMETA(DisplayName = "TwoHandWidthStart")
};