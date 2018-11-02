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
		FTimerHandle OrbWaveFireTimer;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
		FTimerHandle OrbExplosionTimer;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
		class ABoss* OrbOwner;
protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
		int OrbWaveMaxCount;	
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION()
		virtual void FireWave();
	UFUNCTION()
		void DefaultOrbExplosionStart();
	UFUNCTION()
		void ColorOrbExplosionStart();
	UFUNCTION()
		void OwnerSave(class AActor* _OrbOwner);
};
