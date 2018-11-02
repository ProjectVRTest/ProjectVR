// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Monster/Boss/Orb/DefaultOrb/BossOrb.h"
#include "BossYellowOrb.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTVR_API ABossYellowOrb : public ABossOrb
{
	GENERATED_BODY()
public:
	ABossYellowOrb();
protected:
	int a;
	virtual void BeginPlay() override;

	UFUNCTION()
		virtual void FireWave() override;
	
	
};
