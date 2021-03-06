// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "NMWeaponSword.generated.h"

UCLASS()
class PROJECTVR_API ANMWeaponSword : public AActor
{
	GENERATED_BODY()
private:
	UPROPERTY(VisibleAnywhere)
		float Damage;
public:	
	// Sets default values for this actor's properties
	ANMWeaponSword();
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
		class UStaticMeshComponent* SwordMesh;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
		class UStaticMesh* ShovelMesh;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
		class UCapsuleComponent* SwordCollision;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
		bool IsWeaponAttack;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
		bool IsBlockShieldAttack;	
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION()
		void SwordBeginOverlap(UPrimitiveComponent * OverlappedComponent, AActor * OtherActor, UPrimitiveComponent * OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult);
	UFUNCTION()
		float GetDamage();
	UFUNCTION()
		void SetDamage(float NewDamage);	
};
