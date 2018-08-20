// Fill out your copyright notice in the Description page of Project Settings.

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
	PrimaryActorTick.bCanEverTick = true;

	PotionBagMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("PotionBagMesh"));
	SetRootComponent(PotionBagMesh);
	PotionBagMesh->SetRelativeScale3D(FVector(0.117736f, 0.117736f, 0.117736f));

	PotionBagCollision = CreateDefaultSubobject<UCapsuleComponent>(TEXT("PotionBagCollision"));
	PotionBagCollision->SetupAttachment(PotionBagMesh);

	PotionBagCollision->SetRelativeLocation(FVector(0, 10.0f, -50.0f));
	PotionBagCollision->SetRelativeRotation(FRotator(0, 0, 90.0f));
	PotionBagCollision->SetRelativeScale3D(FVector(2.4f, 2.8f, 2.5f));

	static ConstructorHelpers::FObjectFinder<UStaticMesh>SM_PotionBag(TEXT("StaticMesh'/Game/Assets/Item/Mesh/PotionBags/Mesh/PotionBag.PotionBag'"));

	if (SM_PotionBag.Succeeded())
	{
		PotionBagMesh->SetStaticMesh(SM_PotionBag.Object);
	}

	PotionBagMesh->SetCollisionProfileName(TEXT("OverlapAll"));

	PotionCount = 5; //포션의 갯수를 5개로 정함
	PotionBagMesh->ComponentTags.Add(TEXT("PotionBagMesh"));
	Tags.Add(FName("PotionBag"));
	//Tags.Add(FName(TEXT("DisregardForLeftHand")));
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
		//Potion = GetWorld()->SpawnActor<APotion>(Potion->StaticClass(), SpawnActorOption);
		//PotionPush(Potion);	
	}

	if (PotionBagCollision)
	{
		PotionBagCollision->OnComponentBeginOverlap.AddDynamic(this, &APotionBag::PotionBagOverlap);
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

	Potions.Add(PushPotion);

	int CurrentPotionCount = Potions.Num();

	UE_LOG(LogClass, Warning, TEXT("Push : PotionCount %d"), CurrentPotionCount);
	switch (CurrentPotionCount)
	{
	case 1:
		PushPotion->AttachToComponent(PotionBagMesh, AttachRules, TEXT("PotionOne"));
		UE_LOG(LogClass, Warning, TEXT("PotionOne"));
		break;
	case 2:
		PushPotion->AttachToComponent(PotionBagMesh, AttachRules, TEXT("PotionTwo"));
		UE_LOG(LogClass, Warning, TEXT("PotionTwo"));
		break;
	case 3:
		PushPotion->AttachToComponent(PotionBagMesh, AttachRules, TEXT("PotionThree"));
		UE_LOG(LogClass, Warning, TEXT("PotionThree"));
		break;
	case 4:
		PushPotion->AttachToComponent(PotionBagMesh, AttachRules, TEXT("PotionFour"));
		UE_LOG(LogClass, Warning, TEXT("PotionFour"));
		break;
	case 5:
		PushPotion->AttachToComponent(PotionBagMesh, AttachRules, TEXT("PotionFive"));
		UE_LOG(LogClass, Warning, TEXT("PotionFive"));
		break;
	}
}

void APotionBag::PotionBagOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult)
{
	FActorSpawnParameters SpawnActorOption;
	SpawnActorOption.Owner = this;
	SpawnActorOption.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

	if (OtherActor->ActorHasTag(TEXT("Potion")))
	{
		APotion* InputPotion = Cast<APotion>(OtherActor);

		if (InputPotion)
		{
			if (InputPotion->BagInputFlag)
			{
				Potion = GetWorld()->SpawnActor<APotion>(Potion->StaticClass(), SpawnActorOption);
				PotionPush(Potion);
				OtherActor->Destroy();
			}
		}
	}
}
