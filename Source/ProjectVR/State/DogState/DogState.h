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
	Nothing = 5 UMETA(DisplayName = "Nothing")
};

UENUM(BlueprintType)
enum class EDogAnimState : uint8
{
	Idle = 0 UMETA(DisplayName = "Idle"),
	Bark = 1 UMETA(DisplayName = "Bark"),
	Walk = 2 UMETA(DisplayName = "Walk"),
	SideWalk = 3 UMETA(DisplayName = "SideWalk"),
	Run = 4 UMETA(DisplayName = "Run"),
	JumpAttack = 5 UMETA(DisplayName = "JumpAttack"),
	Fly = 6 UMETA(DisplayName = "Fly"),
	StandUp = 7 UMETA(DisplayName = "StandUp"),
	Nothing = 8 UMETA(DisplayName = "Nothing")
};

UENUM(BlueprintType)
enum class EDogJumpState : uint8
{
	JumpStart = 0 UMETA(DisplayName = "JumpStart"),
	JumpRoof = 1 UMETA(DisplayName = "JumpRoof"),
	JumpEnd = 2 UMETA(DisplayName = "JumpEnd"),
	Nothing = 3 UMETA(DisplayName = "Nothing")
};