// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "BossOrbWave.generated.h"

UCLASS()
class PROJECTVR_API ABossOrbWave : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ABossOrbWave();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(VisibleAnywhere, BlueprintReadonly)
		class USphereComponent* Sphere;
	UPROPERTY(VisibleAnywhere, BlueprintReadonly)
		class UProjectileMovementComponent* Projecttile;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Effect")
		class UParticleSystemComponent* OrbWaveParticleComponent;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Effect")
		class UParticleSystem* OrbWaveParticle;

	UPROPERTY(VisibleAnywhere, BlueprintReadonly)
		bool OrbWaveFlag;

	UFUNCTION()
		void Homing(class AActor* Target);

	UFUNCTION()
	void BossOrbWaveBeginOverlap(UPrimitiveComponent * OverlappedComponent, AActor * OtherActor, UPrimitiveComponent * OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult);
};
