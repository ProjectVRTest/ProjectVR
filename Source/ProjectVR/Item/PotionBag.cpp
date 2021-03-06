﻿// Fill out your copyright notice in the Description page of Project Settings.

#include "PotionBag.h"
#include "Components/StaticMeshComponent.h"
#include "Components/CapsuleComponent.h"
#include "UObject/ConstructorHelpers.h"
#include "Item/Potion.h"
#include "Engine/StaticMesh.h"
#include "Engine/World.h"

// Sets default values
APotionBag::APotionBag()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	PotionBagMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("PotionBagMesh"));
	SetRootComponent(PotionBagMesh);
	PotionBagMesh->SetRelativeScale3D(FVector(0.117736f, 0.117736f, 0.117736f));

	PotionBagCollision = CreateDefaultSubobject<UCapsuleComponent>(TEXT("PotionBagCollision"));
	PotionBagCollision->SetupAttachment(PotionBagMesh);

	PotionBagCollision->SetRelativeLocation(FVector(0, 10.0f, -50.0f));
	PotionBagCollision->SetRelativeRotation(FRotator(0, 0, 90.0f));
	PotionBagCollision->SetRelativeScale3D(FVector(2.4f, 2.8f, 2.5f));

	static ConstructorHelpers::FObjectFinder<UStaticMesh>SM_PotionBag(TEXT("StaticMesh'/Game/Assets/CharacterEquipment/Item/PotionBags/Mesh/PotionBag.PotionBag'"));

	if (SM_PotionBag.Succeeded())
	{
		PotionBagMesh->SetStaticMesh(SM_PotionBag.Object);
	}

	PotionBagMesh->SetCollisionProfileName(TEXT("OverlapAll"));

	PotionCount = 5; //포션의 갯수를 5개로 정함
	PotionBagMesh->ComponentTags.Add(TEXT("PotionBagMesh"));
	Tags.Add(FName("PotionBag"));	
}

// Called when the game starts or when spawned
void APotionBag::BeginPlay()
{
	Super::BeginPlay();

	FActorSpawnParameters SpawnActorOption;
	SpawnActorOption.Owner = this;
	SpawnActorOption.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;


	for (int i = 0; i < PotionCount; i++)
	{
		Potion = GetWorld()->SpawnActor<APotion>(Potion->StaticClass(), SpawnActorOption);

		if (Potion)
		{
			PotionPush(Potion);
		}		
	}
}

// Called every frame
void APotionBag::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

APotion* APotionBag::PotionPop()
{
	APotion* PopPotion = Potions.Pop();
	int CurrentPotionCount = Potions.Num();

	UE_LOG(LogClass, Warning, TEXT("Pop : PotionCount %d"), CurrentPotionCount);
	return PopPotion;
}

void APotionBag::PotionPush(APotion* PushPotion)
{
	FAttachmentTransformRules AttachRules(EAttachmentRule::SnapToTarget, EAttachmentRule::SnapToTarget, EAttachmentRule::KeepWorld, false);

	if (PushPotion)
	{
		int index = Potions.Add(PushPotion);

		int CurrentPotionCount = Potions.Num();

		//GLog->Log(FString::Printf(TEXT("Potion Index : %d"), CurrentPotionCount));

		if (PotionBagMesh)
		{
			switch (CurrentPotionCount)
			{
			case 1:
				PushPotion->AttachToComponent(PotionBagMesh, AttachRules, TEXT("PotionOne"));
				break;
			case 2:
				PushPotion->AttachToComponent(PotionBagMesh, AttachRules, TEXT("PotionTwo"));
				break;
			case 3:
				PushPotion->AttachToComponent(PotionBagMesh, AttachRules, TEXT("PotionThree"));
				break;
			case 4:
				PushPotion->AttachToComponent(PotionBagMesh, AttachRules, TEXT("PotionFour"));
				break;
			case 5:
				PushPotion->AttachToComponent(PotionBagMesh, AttachRules, TEXT("PotionFive"));
				break;
			}
		}
	
	}	
}
