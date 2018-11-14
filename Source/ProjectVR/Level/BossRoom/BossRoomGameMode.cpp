// Fill out your copyright notice in the Description page of Project Settings.

#include "BossRoomGameMode.h"
#include "MyCharacter/MotionControllerCharacter.h"
#include "MyCharacter/MotionControllerPC.h"

ABossRoomGameMode::ABossRoomGameMode()
{
	DefaultPawnClass = AMotionControllerCharacter::StaticClass();
	PlayerControllerClass = AMotionControllerPC::StaticClass();
}
