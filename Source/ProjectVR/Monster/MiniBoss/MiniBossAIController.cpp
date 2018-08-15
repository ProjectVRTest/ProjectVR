// Fill out your copyright notice in the Description page of Project Settings.

#include "MiniBossAIController.h"

#include "BehaviorTree/BehaviorTreeComponent.h"
#include "BehaviorTree/BlackboardComponent.h"

#include "MiniBoss.h"

#include "BehaviorTree/BehaviorTree.h"
#include "UObject/ConstructorHelpers.h"

AMiniBossAIController::AMiniBossAIController()
{
	BTComponent = CreateDefaultSubobject<UBehaviorTreeComponent>(TEXT("BehaviorTree"));
	BBComponent = CreateDefaultSubobject<UBlackboardComponent>(TEXT("Blackboard"));	
}

void AMiniBossAIController::Possess(APawn * InPawn)
{
	Super::Possess(InPawn);

	AMiniBoss* MiniBoss = Cast<AMiniBoss>(InPawn);
	
	if (MiniBoss)
	{
		if (MiniBoss->BehaviorTree)
		{
			if (MiniBoss->BehaviorTree->BlackboardAsset)
			{
				BBComponent->InitializeBlackboard(*(MiniBoss->BehaviorTree->BlackboardAsset));
				BTComponent->StartTree(*(MiniBoss->BehaviorTree));
			}
		}
	}
}

