// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Monster/Boss/Orb/BossOrbWave.h"
#include "BossBlueOrbWave.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTVR_API ABossBlueOrbWave : public ABossOrbWave
{
	GENERATED_BODY()
public:
	int a;
	// Sets default values for this actor's properties
	ABossBlueOrbWave();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UFUNCTION()
		virtual void BossOrbWaveBeginOverlap(UPrimitiveComponent * OverlappedComponent, AActor * OtherActor, UPrimitiveComponent * OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult) override;
	
	
	
};
