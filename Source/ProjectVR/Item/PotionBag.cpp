// Fill out your copyright notice in the Description page of Project Settings.

#include "PotionBag.h"
#include "Components/StaticMeshComponent.h"
#include "Components/CapsuleComponent.h"
#include "UObject/ConstructorHelpers.h"
#include "Engine/StaticMesh.h"

// Sets default values
APotionBag::APotionBag()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	PotionBagMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("PotionBagMesh"));
	SetRootComponent(PotionBagMesh);
	PotionBagMesh->SetRelativeScale3D(FVector(0.3f, 0.4f, 0.4f));

	PotionBagCollision = CreateDefaultSubobject<UCapsuleComponent>(TEXT("PotionBagCollision"));
	PotionBagCollision->SetupAttachment(PotionBagMesh);
	
	PotionBagCollision->SetRelativeLocation(FVector(-3.0f, 30.0f, 31.0f));
	PotionBagCollision->SetRelativeScale3D(FVector(0.5f, 0.5f, 0.5f));
	PotionBagCollision->SetRelativeRotation(FRotator(0, 0, 90.0f));
	PotionBagCollision->SetRelativeScale3D(FVector(1.2f, 0.7f, 1.2f));

	static ConstructorHelpers::FObjectFinder<UStaticMesh>SM_PotionBag(TEXT("StaticMesh'/Game/Assets/Item/Mesh/PotionBag.PotionBag'"));

	if (SM_PotionBag.Succeeded())
	{
		PotionBagMesh->SetStaticMesh(SM_PotionBag.Object);
	}

	PotionBagMesh->SetCollisionProfileName(TEXT("OverlapAll"));

	PotionCount = 5; //포션의 갯수를 5개로 정함
	Tags.Add(FName("PotionBag"));
}

// Called when the game starts or when spawned
void APotionBag::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void APotionBag::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

