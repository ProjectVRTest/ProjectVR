// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "MiniBossParryingPoint.generated.h"

UCLASS()
class PROJECTVR_API AMiniBossParryingPoint : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AMiniBossParryingPoint();

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
		class USceneComponent* ParryingPointRoot;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
		class USphereComponent* Sphere;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Effect")
		class UParticleSystem* ParryingPointEffect;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Effect")
		class UParticleSystem* ParryingPointExplosionEffect;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Effect")
		class UParticleSystemComponent* ParryingPointEffectComponent;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Parrying")
		bool IsAttackMiniBossWeapon;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Parrying")
		bool IsAttackBossWeapon;
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION()
	void ParryingPointBeginOverlap(UPrimitiveComponent * OverlappedComponent, AActor * OtherActor, UPrimitiveComponent * OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult);

	
	
};
