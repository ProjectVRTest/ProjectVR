// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "SwordWaveTarget.generated.h"

UCLASS()
class PROJECTVR_API ASwordWaveTarget : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ASwordWaveTarget();
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
		class UBoxComponent* SwordWaveTargetBox;
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	
	
};
