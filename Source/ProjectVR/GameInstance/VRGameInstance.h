// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "VRGameInstance.generated.h"

struct CharacterInfo //캐릭터의 정보를 저장해둘 구조체
{
	int32 CurrentHP; //캐릭터의 HP
	int32 CurrentSP; //캐릭터의 SP
	int32 PotionCount; //캐릭터의 포션갯수
};

/**
 * 
 */
UCLASS()
class PROJECTVR_API UVRGameInstance : public UGameInstance
{
	GENERATED_BODY()
private:
	CharacterInfo MyCharacterInfo; //캐릭터의 정보를 저장해둘 구조체를 변수로 선언
public:
	virtual void Init() override; //처음실행시 실행되는 함수로 구조체를 0으로 초기화해준다.
	virtual void Shutdown() override;//게임이 종료될시 실행되는 함수
	void MyCharacterInfoUpdate(class AMotionControllerCharacter* MyCharacter); //캐릭터의 정보(현재 체력과 스테미나를 업데이트 해주는 함수)

	void PotionCountUpdate(int PotionCount); //캐릭터의 포션갯수를 업데이트 해주는 함수
};
