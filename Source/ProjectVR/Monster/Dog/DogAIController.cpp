// Fill out your copyright notice in the Description page of Project Settings.

#include "DogAIController.h"
#include "BehaviorTree/BehaviorTreeComponent.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Monster/Dog/Dog.h"
#include "BehaviorTree/BehaviorTree.h"


ADogAIController::ADogAIController()
{
	BTComponent = CreateDefaultSubobject<UBehaviorTreeComponent>(TEXT("DogBehaviorTree"));
	BBComponent = CreateDefaultSubobject<UBlackboardComponent>(TEXT("DogBlackboard"));
}

void ADogAIController::Possess(APawn * InPawn)
{
	Super::Possess(InPawn);

	ADog* Dog = Cast<ADog>(InPawn);

	if (Dog)
	{
		if (Dog->BehaviorTree)
		{
			if (Dog->BehaviorTree->BlackboardAsset)
			{
				BBComponent->InitializeBlackboard(*(Dog->BehaviorTree->BlackboardAsset));
				BTComponent->StartTree(*(Dog->BehaviorTree));
			}
		}
	}
}
