// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Monster/Boss/Orb/BossOrb.h"
#include "BossBlueOrb.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTVR_API ABossBlueOrb : public ABossOrb
{
	GENERATED_BODY()

public:
	ABossBlueOrb();

protected:
	virtual void BeginPlay() override;

	UFUNCTION()
		virtual void FireWave() override;	
};
