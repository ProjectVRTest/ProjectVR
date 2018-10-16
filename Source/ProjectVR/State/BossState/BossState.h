// Fill out your copyright notice in the Description page of Project Settings.

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
	AddAttack = 1 UMETA(DisplayName = "AddAttack")
};

UENUM(BlueprintType)
enum class EBossBlinkAttackState : uint8
{
	Idle = 0 UMETA(DisplayName = "Idle"),
	InVisible = 1 UMETA(DisplayName = "InVisible"), //투명화 
	Visible = 2 UMETA(DisplayName = "Visible"), // 플레이어 앞에 나타나면서 불투명화
	BlinkAttack = 3 UMETA(DisplayName = "BlinkAttack")
};
