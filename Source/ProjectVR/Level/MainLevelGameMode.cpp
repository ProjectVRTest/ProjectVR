// Fill out your copyright notice in the Description page of Project Settings.

#include "MainLevelGameMode.h"
#include "MyCharacter/MotionControllerCharacter.h"

AMainLevelGameMode::AMainLevelGameMode()
{
	DefaultPawnClass = AMotionControllerCharacter::StaticClass();
}



