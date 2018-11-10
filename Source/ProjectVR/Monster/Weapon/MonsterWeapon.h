// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "MonsterWeapon.generated.h"

UCLASS()
class PROJECTVR_API AMonsterWeapon : public AActor
{
	GENERATED_BODY()
	
private:
	UPROPERTY(VisibleAnywhere)
		float Damage;
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	AMonsterWeapon();
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
		class UStaticMeshComponent* WeaponMeshComponent;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
		bool IsWeaponAttack;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
		bool IsBlockShieldAttack;

	UFUNCTION()
		float GetDamage();
	UFUNCTION()
		void SetDamage(float NewDamage);
};
