// Fill out your copyright notice in the Description page of Project Settings.

#include "NormalMonsterAIController.h"

#include "BehaviorTree/BehaviorTreeComponent.h"
#include "BehaviorTree/BlackboardComponent.h"

#include "NormalMonster.h"

#include "BehaviorTree/BehaviorTree.h"

ANormalMonsterAIController::ANormalMonsterAIController()
{
	BTComponent = CreateDefaultSubobject<UBehaviorTreeComponent>(TEXT("BehaviorTree"));

	BBComponent = CreateDefaultSubobject<UBlackboardComponent>(TEXT("Blackboard"));
}

void ANormalMonsterAIController::Possess(APawn * InPawn)
{
	Super::Possess(InPawn);

	ANormalMonster* NormalMonster = Cast<ANormalMonster>(InPawn);

	if (NormalMonster)
	{
		if (NormalMonster->SwordBehaviorTree)
		{
			BBComponent->InitializeBlackboard(*(NormalMonster->SwordBehaviorTree->BlackboardAsset));
			BTComponent->StartTree(*(NormalMonster->SwordBehaviorTree));
		}
	}
}
