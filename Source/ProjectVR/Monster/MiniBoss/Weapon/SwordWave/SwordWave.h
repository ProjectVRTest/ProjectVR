// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "SwordWave.generated.h"

UCLASS()
class PROJECTVR_API ASwordWave : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ASwordWave();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	float Damage;
public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
		class USphereComponent* Sphere;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
		class UStaticMeshComponent* Mesh;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
		class UProjectileMovementComponent* Projecttile;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Effect")
		class UParticleSystem* SwordWaveExplosion;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Effect")
		class UParticleSystemComponent* SwordWaveTailComponent;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Effect")
		class UParticleSystem* SwordWaveTail;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Effect")
		class UParticleSystem* SwordWaveGround;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
		class UBoxComponent* SwordWaveHit;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
		FTimerHandle FloorSmokeTimer;
	UFUNCTION()
		void Homing(class AActor* Target);
	UFUNCTION()
		void SwordWaveBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult);
	UFUNCTION()
		void SwordWaveRotatorModify(FRotator NewRotator);
	UFUNCTION()
		void FloorSmokeSpawn();
};
