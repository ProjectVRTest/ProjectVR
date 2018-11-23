// Fill out your copyright notice in the Description page of Project Settings.

#include "MonsterParryingManager.h"
#include "Monster/MiniBoss/MiniBoss.h"
#include "Monster/Boss/Boss.h"
#include "Monster/MonsterParryingPoint.h"
#include "Engine/World.h"
#include "Components/SkeletalMeshComponent.h"

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

void UMonsterParryingManager::ParryingPointSet(ACharacter * Monster, TArray<FName>& _ParryingPoints, float CurrentHP, float MaxHP, int32 &ParryingPointMaxCount)
{
	if (Monster)
	{
		GLog->Log(FString::Printf(TEXT("패링 포인트 정하기 진입")));
		float HPPercent = CurrentHP / MaxHP;
		
		if (HPPercent > 0.5f && HPPercent <= 1.0f)
		{
			GLog->Log(FString::Printf(TEXT("HP가 50프로 이상")));
			int32 PreviousParryingPointName = -1;
			int32 RandomParryingPointName;

			AMonsterParryingPoint* MonsterParryingPoint;
			FActorSpawnParameters SpawnActorOption;
			SpawnActorOption.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

			FAttachmentTransformRules AttachRules(EAttachmentRule::SnapToTarget, EAttachmentRule::SnapToTarget, EAttachmentRule::KeepWorld, false);
			
			RandomParryingPointName = FMath::RandRange(1, 2);

			ParryingPointMaxCount = RandomParryingPointName;

			for (int i = 0; i < ParryingPointMaxCount; i++)
			{
				RandomParryingPointName = FMath::RandRange(0, 6);

				if (RandomParryingPointName == PreviousParryingPointName)
				{
					i--;
					continue;
				}

				MonsterParryingPoint = GetWorld()->SpawnActor<AMonsterParryingPoint>(MonsterParryingPoint->StaticClass(), Monster->GetActorLocation(), Monster->GetActorRotation(), SpawnActorOption);

				if (MonsterParryingPoint)
				{
					FName ParryingPointSpawnLocation = _ParryingPoints[RandomParryingPointName];

					MonsterParryingPoint->AttachToComponent(Monster->GetMesh(), AttachRules, ParryingPointSpawnLocation);

					PreviousParryingPointName = RandomParryingPointName;
				}
			}
		}
		else if (HPPercent > 0.4f &&HPPercent <= 0.5f)
		{
			GLog->Log(FString::Printf(TEXT("HP 40% 이상")));
			ParryingPointMaxCount = 3;
			ParryingPointValueSet(Monster, _ParryingPoints, ParryingPointMaxCount); //3개 랜덤하게 스폰
		}
		else if (HPPercent > 0.3f && HPPercent <= 0.4f)
		{
			GLog->Log(FString::Printf(TEXT("HP 30% 이상")));

			ParryingPointMaxCount = 5;
			ParryingPointValueSet(Monster, _ParryingPoints, ParryingPointMaxCount); //5개 랜덤하게 스폰
		}
		else if (HPPercent >= 0 && HPPercent <= 0.3f)
		{
			GLog->Log(FString::Printf(TEXT("HP 0% 이상")));
			ParryingPointMaxCount = 7;
			ParryingPointValueSet(Monster, _ParryingPoints, ParryingPointMaxCount); //7개 랜덤하게 스폰
		}
	}
}

void UMonsterParryingManager::ParryingPointValueSet(ACharacter * Monster, TArray<FName>& _ParryingPoints, int32 ParryingPointMaxCount)
{
	GLog->Log(FString::Printf(TEXT("HP가 50프로 아래라서 진입")));
	if (Monster)
	{
		int32 RandomParryingPointName;

		TArray<int32> RandomPointNotOverlap;

		bool RandomFlag;

		for (int32 i = 0; i < ParryingPointMaxCount; i++)
		{
			RandomPointNotOverlap.Add(-1);
		}

		AMonsterParryingPoint* MonsterParryingPoint;

		FActorSpawnParameters SpawnActorOption;
		SpawnActorOption.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

		FAttachmentTransformRules AttachRules(EAttachmentRule::SnapToTarget, EAttachmentRule::SnapToTarget, EAttachmentRule::KeepWorld, false);

		for (int32 i = 0; i < ParryingPointMaxCount; i++)
		{
			RandomParryingPointName = FMath::RandRange(0, 6);
			RandomFlag = true;

			for (int32 j = 0; j < ParryingPointMaxCount; j++)
			{
				if (RandomPointNotOverlap[j] == RandomParryingPointName)
				{
					i--;
					RandomFlag = false;
				}
			}

			if (RandomFlag)
			{
				RandomPointNotOverlap[i] = RandomParryingPointName;
			}
		}

		for (int32 i = 0; i < ParryingPointMaxCount; i++)
		{
			MonsterParryingPoint = GetWorld()->SpawnActor<AMonsterParryingPoint>(MonsterParryingPoint->StaticClass(), Monster->GetActorLocation(), Monster->GetActorRotation(), SpawnActorOption);

			if (MonsterParryingPoint)
			{
				FName ParryingPointSpawnLocation = _ParryingPoints[RandomPointNotOverlap[i]];
				MonsterParryingPoint->AttachToComponent(Monster->GetMesh(), AttachRules, ParryingPointSpawnLocation);
			}
		}

		RandomPointNotOverlap.Empty();
	}
}
