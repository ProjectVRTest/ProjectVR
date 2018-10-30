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
		switch (NormalMonster->MonsterKind)
		{
		case ENormalMonsterKind::SwordMan:
			if (NormalMonster->SwordBehaviorTree)
			{
				GLog->Log(FString::Printf(TEXT("SwordMan Start")));
				BBComponent->InitializeBlackboard(*(NormalMonster->SwordBehaviorTree->BlackboardAsset));
				BTComponent->StartTree(*(NormalMonster->SwordBehaviorTree));
			}
			break;
		case ENormalMonsterKind::MoveArcher:
			if (NormalMonster->MoveArcherBehaviorTree)
			{
				GLog->Log(FString::Printf(TEXT("MoveArcher Start")));
				BBComponent->InitializeBlackboard(*(NormalMonster->MoveArcherBehaviorTree->BlackboardAsset));
				BTComponent->StartTree(*(NormalMonster->MoveArcherBehaviorTree));
			}
			break;
		}
		
		NormalMonster->SetEquipment();
	}
}
