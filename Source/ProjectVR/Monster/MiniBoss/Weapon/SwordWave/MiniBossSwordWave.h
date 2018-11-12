// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "MiniBossSwordWave.generated.h"

UCLASS()
class PROJECTVR_API AMiniBossSwordWave : public AActor
{
	GENERATED_BODY()

private:
	UPROPERTY(VisibleAnywhere)
		float Damage;
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
public:
	AMiniBossSwordWave();
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
		class UBoxComponent* SwordBox;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
		class UStaticMeshComponent* Mesh;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
		class UParticleSystemComponent* SwordWaveTailComponent;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
		class UParticleSystem* SwordWaveTail;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
		class UParticleSystem* SwordWaveExplosion;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
		class UParticleSystem* SwordWaveGround;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
		class UProjectileMovementComponent* Projecttile;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
		FTimerHandle FloorSmokeTimer;

	UFUNCTION()
		void Homing(class AActor* Target);
	UFUNCTION()
		float GetDamage();
	UFUNCTION()
		void SetDamage(float NewDamage);
	UFUNCTION()
		void SwordWaveBeginOverlap(UPrimitiveComponent * OverlappedComponent, AActor * OtherActor, UPrimitiveComponent * OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult);
	UFUNCTION()
		void FloorSmokeSpawn();
};
