// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BossState.generated.h"

UENUM(BlueprintType)
enum class EBossState : uint8
{
	Idle = 0 UMETA(DisplayName = "Idle"),	
	Battle = 1 UMETA(DisplayName = "Battle"),
	Dead = 2 UMETA(DisplayName = "Dead"),
	Groggy = 3 UMETA(DisplayName = "Groggy")
};


