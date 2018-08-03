// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "NormalMonsterAIController.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTVR_API ANormalMonsterAIController : public AAIController
{
	GENERATED_BODY()
public:
	ANormalMonsterAIController();
	
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "AI")
		class UBehaviorTreeComponent* BTComponent;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "AI")
		class UBlackboardComponent* BBComponent;

	UFUNCTION()
		virtual void Possess(APawn* InPawn) override;
};
