// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "NavigationAIController.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTVR_API ANavigationAIController : public AAIController
{

	GENERATED_BODY()

public:
		ANavigationAIController();

	UPROPERTY(VisibleAnywhere, Category = "AI")
		class UBehaviorTreeComponent* BTComponent;

	UPROPERTY(VisibleAnywhere, Category = "AI")
		class UBlackboardComponent* BBComponent;

	UFUNCTION()
		virtual void Possess(APawn* InPawn) override;
	
	
};
