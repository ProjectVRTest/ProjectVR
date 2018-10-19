// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "BossOrb.generated.h"

UCLASS()
class PROJECTVR_API ABossOrb : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	ABossOrb();
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Effect")
		class UParticleSystemComponent* OrbParticleComponent;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Effect")
		class UParticleSystem* OrbParticle;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Effect")
		class UParticleSystem* OrbExplosion;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
		class USphereComponent* Sphere;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
		class USceneComponent* OrbWaveSpawn;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
		int OrbWaveMaxCount;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
		FTimerHandle OrbWaveFireTimer;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
		FTimerHandle OrbExplosionTimer;
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION()
		void FireWave();
	UFUNCTION()
		void OrbExplosionStart();
};
