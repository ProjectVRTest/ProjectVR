// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "PotionBag.generated.h"

UCLASS()
class PROJECTVR_API APotionBag : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	APotionBag();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
		class UStaticMeshComponent* PotionBagMesh; //포션가방 메쉬

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
		class UCapsuleComponent* PotionBagCollision; //포션 컬리젼
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Potion")
		int PotionCount; //포션의 갯수
};
