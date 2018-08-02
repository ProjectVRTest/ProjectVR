// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "DogAIController.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTVR_API ADogAIController : public AAIController
{
	GENERATED_BODY()
	
public:
	ADogAIController();

	UPROPERTY(VisibleAnywhere, Category = "AI")
		class UBehaviorTreeComponent* BTComponent;

	UPROPERTY(VisibleAnywhere, Category = "AI")
		class UBlackboardComponent* BBComponent;

	bool test;

	UFUNCTION()
		virtual void Possess(APawn* InPawn) override;
	
	
};
