// Fill out your copyright notice in the Description page of Project Settings.

#include "BossAIController.h"
#include "BehaviorTree/BehaviorTreeComponent.h"
#include "BehaviorTree/BlackboardComponent.h"

#include "Boss.h"

#include "BehaviorTree/BehaviorTree.h"
#include "UObject/ConstructorHelpers.h"

ABossAIController::ABossAIController()
{
	BTComponent = CreateDefaultSubobject<UBehaviorTreeComponent>(TEXT("BehaviorTree"));
	BBComponent = CreateDefaultSubobject<UBlackboardComponent>(TEXT("Blackboard"));
}

void ABossAIController::Possess(APawn * InPawn)
{
	Super::Possess(InPawn);

	ABoss* Boss = Cast<ABoss>(InPawn);

	if (Boss)
	{
		if (Boss->BehaviorTree)
		{
			if (Boss->BehaviorTree->BlackboardAsset)
			{
				BBComponent->InitializeBlackboard(*(Boss->BehaviorTree->BlackboardAsset));
				BTComponent->StartTree(*(Boss->BehaviorTree));
			}
		}
	}
}
