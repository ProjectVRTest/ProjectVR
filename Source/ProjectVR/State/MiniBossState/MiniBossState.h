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
	AttackReady = 5 UMETA(DisplayName = "AttackReady"),
	ParryingReady = 6 UMETA(DisplayName = "ParryingReady"),
	ParryingStart = 7 UMETA(DisplayName = "ParryingStart"),
	BackWalk = 8 UMETA(DisplayName = "BackWalk"),                                               
	DashAttack = 9 UMETA(DisplayName = "DashAttack"),
	Rotate = 10 UMETA(DisplayName = "Rotate"),
	BattleWalk = 11 UMETA(DisplayName = "BattleWalk")
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
enum class EMiniBossShortAttackState : uint8
{
	Idle = 0 UMETA(DisplayName = "Idle"),
	ShortAttackReady = 1 UMETA(DisplayName = "ShortAttackReady"),
	RightUpLeftDownReady = 2 UMETA(DisplayName = "RightUpLeftDownReady"),
	RightUpLeftDownStart = 3 UMETA(DisplayName = "RightUpLeftDownStart"),
	TwoHandWidthReady = 4 UMETA(DisplayName = "TwoHandWidthReady"),
	TwoHandWidthStart = 5 UMETA(DisplayName = "TwoHandWidthStart"),
	StabReady = 6 UMETA(DisplayName = "StabReady"),
	StabStart = 7 UMETA(DisplayName = "StabStart"),
	CounterAttack = 8 UMETA(DisplayName = "CounterAttack")
};

UENUM(BlueprintType)
enum class EMiniBossWaveAttackState : uint8
{
	Idle = 0 UMETA(DisplayName = "Idle"),
	TwoHandWidthReady = 1 UMETA(DisplayName = "TwoHandWidthReady"),
	TwoHandWidthStart = 2 UMETA(DisplayName = "TwoHandWidthStart"),
	SwordWaveReady = 3 UMETA(DisplayName = "SwordWaveReady"),
	SwordWaveStart = 4 UMETA(DisplayName = "SwordWaveStart")
};

UENUM(BlueprintType)
enum class EMiniBossComboAttackState : uint8
{
	Idle = 0 UMETA(DisplayName = "Idle"),
	ComboAttack = 1 UMETA(DisplayName = "ComboAttack")
};

UENUM(BlueprintType)
enum class EMiniBossBackAttackState : uint8
{
	Idle = 0 UMETA(DisplayName = "Idle"),
	BackAttack = 1 UMETA(DisplayName = "BackAttack")
};

