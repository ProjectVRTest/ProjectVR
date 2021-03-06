// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "NMWeaponArrow.generated.h"

UCLASS()
class PROJECTVR_API ANMWeaponArrow : public AActor
{
	GENERATED_BODY()

private:
	UPROPERTY(VisibleAnywhere)
		float Damage;
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
public:	
	ANMWeaponArrow();
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
		class UStaticMeshComponent* Mesh;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
		class UProjectileMovementComponent* Projecttile;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
		class UBoxComponent* ArrowHit;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
		class UParticleSystemComponent* ArrowEffectComponent;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Effect")
		class UParticleSystem* ArrowEffect;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Effect")
		class UParticleSystem* ArrowExplosionEffect;
	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION()
	void ArrowBeginOverlap(UPrimitiveComponent * OverlappedComponent, AActor * OtherActor, UPrimitiveComponent * OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult);

	UFUNCTION()
		void Homing(class AActor* Target);
	
	UFUNCTION()
		float GetDamage();
	UFUNCTION()
		void SetDamage(float NewDamage);
};
