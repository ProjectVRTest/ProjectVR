// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "NormalMonsterState.generated.h"

UENUM(BlueprintType)
enum class ENormalMonsterKind : uint8
{
	SwordMan = 0 UMETA(DisplayName = "SwordMan"),
	MoveArcher = 1 UMETA(DisplayName = "MoveArcher"),
	DontMoveArcher = 2 UMETA(DisplayName = "DontMoveArcher")
};

UENUM(BlueprintType)
enum class ENormalMonsterState : uint8
{
	Idle = 0 UMETA(DisplayName = "Idle"),
	Patrol = 1 UMETA(DisplayName = "Patrol"),
	Chase = 2 UMETA(DisplayName = "Chase"),
	Battle = 3 UMETA(DisplayName = "Battle"),
	Dead = 4 UMETA(DisplayName = "Dead"),
	AttackWait = 5 UMETA(DisplayName = "AttackWait")
};

UENUM(BlueprintType)
enum class ENormalMonsterAnimState : uint8
{
	Wait =0 UMETA(DisplayName = "Wait"),
	Walk = 1 UMETA(DisplayName = "Walk"),
	Run = 2 UMETA(DisplayName = "Run"),
	RageRun = 3 UMETA(DisplayName = "RageRun"),
	RageAttack = 4 UMETA(DisplayName = "RageAttack"),
	AttackReady = 5 UMETA(DisplayName = "AttackReady"),
	Attack = 6 UMETA(DisplayName = "Attack")
};

UENUM(BlueprintType)
enum class ENormalMonsterIdleState : uint8
{
	Wait = 0 UMETA(DisplayName = "Wait"),
	Lean = 1 UMETA(DisplayName = "Lean"),
	Recumbent = 2 UMETA(DisplayName = "Recumbent"),
	SitDown = 3 UMETA(DisplayName = "SitDown"),
	Hide = 4 UMETA(DisplayName= "Hide")
};

UENUM(BlueprintType)
enum class ENormalMonsterAttackState : uint8
{
	Idle = 0 UMETA(DisplayName = "Idle"),
	StabAttack = 1 UMETA(DisplayName = "StabAttack"),	
	ComboAttack = 2 UMETA(DisplayName = "ComboAttack")	,
	BowAttack = 3 UMETA(DisplayName = "BowAttack")
};

UENUM(BlueprintType)
enum class ENormalMonsterStabAttackState : uint8
{
	Idle = 0 UMETA(DisplayName = "Idle"),
	StabAttackReady = 1 UMETA(DisplayName = "StabAttackReady"),
	StabAttackStart = 2 UMETA(DisplayName = "StabAttackStart"),
	ClipplingAttackReady = 3 UMETA(DisplayName = "ClipplingAttackReady"),
	ClipplingAttackStart = 4 UMETA(DisplayName = "ClipplingAttackStart")
};

UENUM(BlueprintType)
enum class ENormalMonsterComboAttackState : uint8
{
	Idle = 0 UMETA(DisplayName = "Idle"),
	ComboAttackReady = 1 UMETA(DisplayName = "ComboAttackReady"),	
	ComboAttackStart = 2 UMETA(DisplayName = "ComboAttackStart")
};

UENUM(BlueprintType)
enum class ENormalMonsterArcherAttackState : uint8
{
	idle = 0 UMETA(DisplayName = "Idle"),
	ArcherAttackReady = 1 UMETA(DisplayName = "ArcherAttackReady"),
	ArcherAttackStart = 2 UMETA(DisplayName = "ArcherAttackStart")
};