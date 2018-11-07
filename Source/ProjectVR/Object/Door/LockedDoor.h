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

	// Door
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
		class USceneComponent* Scene;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
		class USceneComponent* DoorScene1;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
		class USceneComponent* DoorScene2;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
		class UStaticMeshComponent* Door1;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
		class UStaticMeshComponent* Door2;

	UPROPERTY()
		FRotator AutoRot;

	UPROPERTY()
		float DefaultYaw;
	UPROPERTY()
		class AActor* TouchActor;

	bool bGetKey;		// true면 열림

	UFUNCTION()
		void OpenDoor();

	// Lock
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "LockScene")
		class USceneComponent* Scene2;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "LockScene")
		class USceneComponent* LockScene;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "LockScene")
		class USceneComponent* CollisionScene;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Lock")
		class UStaticMeshComponent* Body;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Lock")
		class UStaticMeshComponent* Button;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Lock")
		class UStaticMeshComponent* Opener;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Lock")
		class UStaticMeshComponent* Chain;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "LockCollision")
		class UBoxComponent* BoxCollision;

	UPROPERTY()
		class ALockKey* Key;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Scene")
		class USceneComponent* KeySocket;

	FTimerHandle OpenHandle;			// 타이머핸들

	UFUNCTION()
		void OpenDoors();

	UFUNCTION()
		void GetLockKey();
};
