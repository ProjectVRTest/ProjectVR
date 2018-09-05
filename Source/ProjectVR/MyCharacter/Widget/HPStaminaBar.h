// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "HPStaminaBar.generated.h"

UCLASS()
class PROJECTVR_API AHPStaminaBar : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AHPStaminaBar();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Cateroty = "HPBar")
		class USceneComponent* HPScene;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Cateroty = "HPBar")
		class UBoxComponent* HPBar;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Cateroty = "StaminaBar")
		class USceneComponent* SteminaScene;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Cateroty = "StaminaBar")
		class UBoxComponent* StaminaBar;

	UPROPERTY(VisibleAnywhere, Category = "Size")
		float HPBarXSize;
	UPROPERTY(VisibleAnywhere, Category = "Size")
		float HPBarZSize;
	UPROPERTY(VisibleAnywhere, Category = "Size")
		float SteminaBarXSize;
	UPROPERTY(VisibleAnywhere, Category = "Size")
		float SteminaBarZSize;
};
