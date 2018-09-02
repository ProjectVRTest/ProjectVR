// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "MiniBossWeapon.generated.h"

UCLASS()
class PROJECTVR_API AMiniBossWeapon : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AMiniBossWeapon();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION()
	void WeaponBeginOverlap(UPrimitiveComponent * OverlappedComponent, AActor * OtherActor, UPrimitiveComponent * OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult);

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
		class UStaticMeshComponent* SwordMesh;			
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
		class UCapsuleComponent* SwordCollision;				
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
		bool IsWeaponAttack;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
		bool IsParryingAttack;
};
