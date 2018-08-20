// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Monster/MiniBoss/MiniBossAIController.h"
#include "Monster/MiniBoss/MiniBoss.h"

#include "BehaviorTree/BlackboardComponent.h"
#include "TimerManager.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "kismet/KismetMathLibrary.h"
#include "kismet/GameplayStatics.h"
#include "MyCharacter/MotionControllerCharacter.h"
#include "Camera/CameraComponent.h"
#include "Monster/MiniBoss/Weapon/MiniBossWeapon.h"

#define RightUpLeftDownAttack 1
#define TwoHandWidthAttack 2
#define StabAttack 3