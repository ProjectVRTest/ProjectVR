// Fill out your copyright notice in the Description page of Project Settings.

#include "MonsterParryingManager.h"
#include "Monster/MiniBoss/MiniBoss.h"
#include "Monster/Boss/Boss.h"

// Sets default values for this component's properties
UMonsterParryingManager::UMonsterParryingManager()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;

	// ...
}


// Called when the game starts
void UMonsterParryingManager::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}


// Called every frame
void UMonsterParryingManager::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

void UMonsterParryingManager::ParryingPointInit(AActor * Monster, TArray<FName>& _ParryingPoints)
{
	AMiniBoss* MiniBoss = Cast<AMiniBoss>(Monster);

	if (MiniBoss)
	{
		_ParryingPoints.Add(TEXT("RightArmsParryingPoint"));
		_ParryingPoints.Add(TEXT("RightUpperArmsParryingPoint"));
		_ParryingPoints.Add(TEXT("LeftArmsParryingPoint"));
		_ParryingPoints.Add(TEXT("LeftUpperArmsParryingPoint"));
		_ParryingPoints.Add(TEXT("SpineMiddleParryingPoint"));
		_ParryingPoints.Add(TEXT("LeftKneeParryingPoint"));
		_ParryingPoints.Add(TEXT("RightKneeParryingPoint"));
	}
	else
	{
		ABoss* Boss = Cast<ABoss>(Monster);

		if (Boss)
		{
			_ParryingPoints.Add(TEXT("HeadParryingPoint"));
			_ParryingPoints.Add(TEXT("RightHandParryingPoint"));
			_ParryingPoints.Add(TEXT("LeftHandParryingPoint"));
			_ParryingPoints.Add(TEXT("LeftHandFingerParryingPoint"));
			_ParryingPoints.Add(TEXT("SpineParryingPoint"));
			_ParryingPoints.Add(TEXT("LeftKneeParryingPoint"));
			_ParryingPoints.Add(TEXT("RightKneeParryingPoint"));
		}
	}
}
