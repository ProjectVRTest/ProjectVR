// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GripEnum.generated.h"

UENUM(BlueprintType)
enum class E_HandState :uint8
{
	Open = 0 UMETA(DisplayName = "Open"),
	CanGrab = 1 UMETA(DisplayName = "CanGrab"),
	Grab = 2 UMETA(DisplayName = "Grab")
};

UENUM(BlueprintType)
enum class EHandFormState : uint8
{
	WeaponHandGrab = 0 UMETA(DisplayName="WeaponHandGrab"),
	PotionHandGrab = 1 UMETA(DisplayName="PotionHandGrab")
};
