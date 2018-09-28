// Fill out your copyright notice in the Description page of Project Settings.

#include "CameraLocation.h"
#include "Components/SphereComponent.h"

// Sets default values
ACameraLocation::ACameraLocation()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Sphere = CreateDefaultSubobject<USphereComponent>(TEXT("Sphere"));
	SetRootComponent(Sphere);
	Sphere->SetCollisionProfileName("OverlapAll");	
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

