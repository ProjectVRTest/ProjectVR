// Fill out your copyright notice in the Description page of Project Settings.

#include "MainLevelGameMode.h"
#include "MyCharacter/MotionControllerCharacter.h"
#include "MyCharacter/MotionControllerPC.h"

AMainLevelGameMode::AMainLevelGameMode()
{
	DefaultPawnClass = AMotionControllerCharacter::StaticClass();
	PlayerControllerClass = AMotionControllerPC::StaticClass();
}



