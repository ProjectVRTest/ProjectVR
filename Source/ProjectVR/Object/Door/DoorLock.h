// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "DoorLock.generated.h"

DECLARE_DELEGATE(FOpenEvent);

UCLASS()
class PROJECTVR_API ADoorLock : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ADoorLock();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Scene")
		class USceneComponent* Scene;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Scene")
		class USceneComponent* LockScene;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Scene")
		class USceneComponent* CollisionScene;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Lock")
		class UStaticMeshComponent* Body;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Lock")
		class UStaticMeshComponent* Button;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Lock")
		class UStaticMeshComponent* Opener;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Lock")
		class UStaticMeshComponent* Chain;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "LockCollision")
		class UBoxComponent* BoxCollision;

	UPROPERTY()
		class ALockKey* Key;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Scene")
		class USceneComponent* KeySocket;

	FTimerHandle OpenHandle;			// 타이머핸들

	FOpenEvent OpenEvent;

	UFUNCTION()
		void OpenDoor();

	UFUNCTION()
		void GetLockKey();
};
