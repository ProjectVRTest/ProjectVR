// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Monster/Boss/Orb/BossOrb.h"
#include "BossRedOrb.generated.h"

/**
 *
 */
UCLASS()
class PROJECTVR_API ABossRedOrb : public ABossOrb
{
	GENERATED_BODY()

public:
	ABossRedOrb();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UFUNCTION()
		virtual void FireWave() override;


};
