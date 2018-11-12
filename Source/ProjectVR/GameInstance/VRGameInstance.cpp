// Fill out your copyright notice in the Description page of Project Settings.

#include "VRGameInstance.h"
#include "MyCharacter/MotionControllerCharacter.h"
#include "Item/Table/ItemDataSingleton.h"

void UVRGameInstance::Init()
{
	Super::Init();
	memset(&MyCharacterInfo, 0, sizeof(CharacterInfo));
}

void UVRGameInstance::Shutdown()
{
	Super::Shutdown();
	if (AItemDataSingleton::GetInstatnce() != nullptr)
	{
		AItemDataSingleton::Destroy();
	}
}

void UVRGameInstance::MyCharacterInfoUpdate(AMotionControllerCharacter* MyCharacter)
{
	MyCharacterInfo.CurrentHP = MyCharacter->CurrentHp;
	MyCharacterInfo.CurrentSP = MyCharacter->CurrentStamina;
}

void UVRGameInstance::PotionCountUpdate(int PotionCount)
{
	MyCharacterInfo.PotionCount = PotionCount;
}
