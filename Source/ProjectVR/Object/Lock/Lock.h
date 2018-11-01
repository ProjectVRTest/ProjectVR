// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Lock.generated.h"

DECLARE_DELEGATE(FOpenEvent);

UCLASS()
class PROJECTVR_API ALock : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ALock();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(EditAnywhere)
		class USceneComponent* Scene;
	UPROPERTY(EditAnywhere)
		class UStaticMeshComponent* Body;
	UPROPERTY(EditAnywhere)
		class UStaticMeshComponent* Button;
	UPROPERTY(EditAnywhere)
		class UStaticMeshComponent* Opener;
	UPROPERTY(EditAnywhere)
		class UStaticMeshComponent* Chain;

	FOpenEvent OpenEvent;

	UFUNCTION()
		void OnOverlap(UPrimitiveComponent * OverlappedComp, AActor * OtherActor, UPrimitiveComponent * OtherComp,
			int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult);		// 오버랩이벤트로 오른손과 상호작용하기 위해서 만듦
};
