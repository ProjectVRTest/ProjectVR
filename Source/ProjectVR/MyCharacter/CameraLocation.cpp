// Fill out your copyright notice in the Description page of Project Settings.

#include "CameraLocation.h"
#include "Components/SphereComponent.h"
#include "Components/CapsuleComponent.h"

// Sets default values
ACameraLocation::ACameraLocation()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	/*Capsule = CreateDefaultSubobject<UCapsuleComponent>(TEXT("Capsule"));
	SetRootComponent(Capsule);
	Capsule->SetRelativeScale3D(FVector(1.5f, 1.5f, 2.0f));
	Capsule->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	Capsule->SetCollisionObjectType(ECollisionChannel::ECC_GameTraceChannel3);
	Capsule->SetCollisionResponseToChannel(ECollisionChannel::ECC_GameTraceChannel1, ECollisionResponse::ECR_Block);
	Capsule->SetCollisionResponseToChannel(ECollisionChannel::ECC_GameTraceChannel2, ECollisionResponse::ECR_Overlap);
	Capsule->bHiddenInGame = false;*/
	
	Sphere = CreateDefaultSubobject<USphereComponent>(TEXT("Sphere"));
	SetRootComponent(Sphere);
	//Sphere->SetupAttachment(Capsule);
	Sphere->SetCollisionProfileName("OverlapAll");
	Sphere->bHiddenInGame = false;

	Sphere->ComponentTags.Add(FName(TEXT("CameraLocation")));
	Sphere->ComponentTags.Add(FName(TEXT("DisregardForLeftHand")));
	Sphere->ComponentTags.Add(FName(TEXT("DisregardForRightHand")));

	//Capsule->ComponentTags.Add(FName(TEXT("DisregardForLeftHand")));
	//Capsule->ComponentTags.Add(FName(TEXT("DisregardForRightHand")));

	Tags.Add(FName(TEXT("DisregardForLeftHand")));
	Tags.Add(FName(TEXT("DisregardForRightHand")));
}

// Called when the game starts or when spawned
void ACameraLocation::BeginPlay()
{
	Super::BeginPlay();	
}

// Called every frame
void ACameraLocation::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}
