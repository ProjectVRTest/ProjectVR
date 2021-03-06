// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "BossWeapon.generated.h"

UCLASS()
class PROJECTVR_API ABossWeapon : public AActor
{
	GENERATED_BODY()
private:
	UPROPERTY(VisibleAnywhere)
		float Damage;
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;	
public:	
	ABossWeapon();

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
		class UStaticMeshComponent* SwordMesh;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
		bool IsWeaponAttack;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
		bool IsParryingAttack;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Materials")
		class UMaterialInterface* DefaultSwordMaterials;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
		class UCapsuleComponent* SwordCollision;
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION()
	void WeaponBeginOverlap(UPrimitiveComponent * OverlappedComponent, AActor * OtherActor, UPrimitiveComponent * OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult);

	UFUNCTION()
		float GetDamage();
	UFUNCTION()
		void SetDamage(float NewDamage);
	
};
