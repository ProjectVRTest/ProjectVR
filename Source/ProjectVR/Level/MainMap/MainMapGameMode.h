// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "MainMapGameMode.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTVR_API AMainMapGameMode : public AGameModeBase
{
	GENERATED_BODY()
public:
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite,Category = "SpawnNormalMonsterCount")
		int32 NormalMonsterCount;
	AMainMapGameMode();	

	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;
};
