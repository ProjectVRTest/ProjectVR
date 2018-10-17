// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "BossAIController.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTVR_API ABossAIController : public AAIController
{
	GENERATED_BODY()
public:
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "AI")
		class UBehaviorTreeComponent* BTComponent;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "AI")
		class UBlackboardComponent* BBComponent;

	ABossAIController();
	UFUNCTION()
		virtual void Possess(APawn* InPawn) override;

};
