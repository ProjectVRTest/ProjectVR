// Fill out your copyright notice in the Description page of Project Settings.

#include "DontMoveArcherAIController.h"

#include "BehaviorTree/BehaviorTreeComponent.h"
#include "BehaviorTree/BlackboardComponent.h"

#include "DontMoveArcher.h"

#include "BehaviorTree/BehaviorTree.h"

ADontMoveArcherAIController::ADontMoveArcherAIController()
{
	BTComponent = CreateDefaultSubobject<UBehaviorTreeComponent>(TEXT("BehaviorTree"));

	BBComponent = CreateDefaultSubobject<UBlackboardComponent>(TEXT("Blackboard"));
}

void ADontMoveArcherAIController::Possess(APawn * InPawn)
{
	Super::Possess(InPawn);

	ADontMoveArcher* DontMoveArcher = Cast<ADontMoveArcher>(InPawn);

	if (DontMoveArcher)
	{
		BBComponent->InitializeBlackboard(*(DontMoveArcher->BehaviorTree->BlackboardAsset));
		BTComponent->StartTree(*(DontMoveArcher->BehaviorTree));
	}
}
