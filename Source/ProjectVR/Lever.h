// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Lever.generated.h"

UCLASS()
class PROJECTVR_API ALever : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ALever();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
		class USceneComponent* LeverScene;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
		class UStaticMeshComponent* Lever;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
		class UStaticMeshComponent* LeverObject;

	UPROPERTY()
		class AActor* Interactor;

	UPROPERTY()
		class AActor* TouchActor;

	UFUNCTION()
		void OnLeverOverlap(UPrimitiveComponent * OverlappedComp, AActor * OtherActor, UPrimitiveComponent * OtherComp,
			int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult);		// 오버랩이벤트로 오른손과 상호작용하기 위해서 만듦

	UFUNCTION()
		void OnLeverEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor
			, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);		// 만약 오른손이 레버를 빠져나간다면 Interactor를 null로 만들어줘야하기 때문

};
