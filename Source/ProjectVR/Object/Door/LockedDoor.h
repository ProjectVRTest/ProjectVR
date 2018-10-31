// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "LockedDoor.generated.h"

UCLASS()
class PROJECTVR_API ALockedDoor : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ALockedDoor();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
		class USceneComponent* Scene;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
		class UStaticMeshComponent* DoorMesh;

	//UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	//	class UBoxComponent* Collision;

	UPROPERTY()
		FRotator AutoRot;

	UPROPERTY()
		float DefaultYaw;
	UPROPERTY()
		class AActor* TouchActor;

	bool bGetKey;

	UFUNCTION()
		void OpenDoor();
};
