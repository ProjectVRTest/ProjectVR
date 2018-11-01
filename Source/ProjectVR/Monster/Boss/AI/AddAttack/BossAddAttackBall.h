// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "BossAddAttackBall.generated.h"

UCLASS()
class PROJECTVR_API ABossAddAttackBall : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	ABossAddAttackBall();
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(VisibleAnywhere, BlueprintReadonly)
		class UStaticMeshComponent* Mesh;
	UPROPERTY(VisibleAnywhere, BlueprintReadonly)
		class USphereComponent* Sphere;
	UPROPERTY(VisibleAnywhere, BlueprintReadonly)
		class UProjectileMovementComponent* Projecttile;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Effect")
		class UParticleSystem* AddAttackBallEffect;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Effect")
		class UParticleSystemComponent* AddAttackBallEffectComponent;

	UFUNCTION()
		void Homing(class AActor* Target);
	UFUNCTION()
		void AttackBallBeginOverlap(UPrimitiveComponent * OverlappedComponent, AActor * OtherActor, UPrimitiveComponent * OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult);

};
