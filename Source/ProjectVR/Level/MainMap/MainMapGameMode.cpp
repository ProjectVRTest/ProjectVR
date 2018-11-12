// Fill out your copyright notice in the Description page of Project Settings.

#include "MainMapGameMode.h"
#include "MyCharacter/MotionControllerCharacter.h"
#include "MyCharacter/MotionControllerPC.h"

AMainMapGameMode::AMainMapGameMode()
{
	DefaultPawnClass = AMotionControllerCharacter::StaticClass();
	PlayerControllerClass = AMotionControllerPC::StaticClass();

	PrimaryActorTick.bCanEverTick = true;
}

void AMainMapGameMode::BeginPlay()
{
	Super::BeginPlay();

	NormalMonsterCount = 9;
		
}

void AMainMapGameMode::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	GLog->Log(FString::Printf(TEXT("NormalMonsterCount : %d"), NormalMonsterCount));
}
