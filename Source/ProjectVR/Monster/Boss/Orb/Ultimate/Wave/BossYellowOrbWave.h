// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Monster/Boss/Orb/DefaultOrb/BossOrbWave.h"
#include "BossYellowOrbWave.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTVR_API ABossYellowOrbWave : public ABossOrbWave
{
	GENERATED_BODY()
public:
	int a;
	ABossYellowOrbWave();
protected:
	virtual void BeginPlay() override;
	
	UFUNCTION()
		virtual void BossOrbWaveBeginOverlap(UPrimitiveComponent * OverlappedComponent, AActor * OtherActor, UPrimitiveComponent * OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult) override;
};