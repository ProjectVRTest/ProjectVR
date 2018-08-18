// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

UENUM(BlueprintType)
enum class EDogState : uint8
{
	Idle = 0 UMETA(DisplayName = "Idle"),
	Chase = 1 UMETA(DisplayName = "Chase"),
	Battle = 2 UMETA(DisplayName = "Battle"),
	Death = 3 UMETA(DisplayName = "Death"),
	Hurled = 4 UMETA(DisplayName = "Hurled"),
	Nothing = 5 UMETA(DisplayName = "Nothing"),
	Circle = 6 UMETA(DisplayName = "Circle"),
};

UENUM(BlueprintType)
enum class EDogAnimState : uint8
{
	Idle = 0 UMETA(DisplayName = "Idle"),
	Bark = 1 UMETA(DisplayName = "Bark"),
	Walk = 2 UMETA(DisplayName = "Walk"),
	RightSideWalk = 3 UMETA(DisplayName = "RightSideWalk"),
	Run = 4 UMETA(DisplayName = "Run"),
	JumpAttack = 5 UMETA(DisplayName = "JumpAttack"),
	Fly = 6 UMETA(DisplayName = "Fly"),
	StandUp = 7 UMETA(DisplayName = "StandUp"),
	Nothing = 8 UMETA(DisplayName = "Nothing"),
	LeftSideWalk = 9 UMETA(DisplayName = "LeftSideWalk"),
	SideWalk = 10 UMETA(DisplayName = "SideWalk"),
	FallingDeath = 11 UMETA(DisplayName = "FallingDeath"),
	StandDeath = 12 UMETA(DisplayName = "StandDeath")
};

UENUM(BlueprintType)
enum class EDogJumpState : uint8
{
	JumpStart = 0 UMETA(DisplayName = "JumpStart"),
	JumpRoof = 1 UMETA(DisplayName = "JumpRoof"),
	JumpEnd = 2 UMETA(DisplayName = "JumpEnd"),
	Nothing = 3 UMETA(DisplayName = "Nothing")
};

UENUM(BlueprintType)
enum class EDogCircleState : uint8
{
	LeftCircle = 0 UMETA(DisplayName = "LeftCircle"),
	RightCircle = 1 UMETA(DisplayName = "RightCircle"),
	Nothing = 3 UMETA(DisplayName = "Nothing")
};