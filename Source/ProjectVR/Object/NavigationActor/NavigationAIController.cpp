// Fill out your copyright notice in the Description page of Project Settings.

#include "NavigationAIController.h"
#include "Object/NavigationActor/NavigationActor.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "BehaviorTree/BehaviorTree.h"

ANavigationAIController::ANavigationAIController()
{
	BTComponent = CreateDefaultSubobject<UBehaviorTreeComponent>(TEXT("DogBehaviorTree"));
	BBComponent = CreateDefaultSubobject<UBlackboardComponent>(TEXT("DogBlackboard"));
}

void ANavigationAIController::Possess(APawn * InPawn)
{
	Super::Possess(InPawn);

	ANavigationActor* Navi_Actor = Cast<ANavigationActor>(InPawn);

	if (Navi_Actor)
	{
		if (Navi_Actor->BehaviorTree)
		{
			if (Navi_Actor->BehaviorTree->BlackboardAsset)
			{
				BBComponent->InitializeBlackboard(*(Navi_Actor->BehaviorTree->BlackboardAsset));
				BTComponent->StartTree(*(Navi_Actor->BehaviorTree));
			}
		}
	}
}
