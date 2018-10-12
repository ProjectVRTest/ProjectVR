// Fill out your copyright notice in the Description page of Project Settings.

#include "NavigationPoint.h"
#include "Components/BoxComponent.h"
ANavigationPoint::ANavigationPoint()
{

	Collision = CreateDefaultSubobject<UBoxComponent>(TEXT("Collision"));
	Collision->SetupAttachment(RootComponent);

	Collision->SetRelativeScale3D(FVector(1.0f, 1.0f, 1.0f));
	Collision->bGenerateOverlapEvents = true;
	Collision->SetCollisionProfileName("OverlapAll");

	Tags.Add(FName("NavigationEvent"));
}

void ANavigationPoint::BeginPlay()
{
	Super::BeginPlay();

	Collision->OnComponentBeginOverlap.AddDynamic(this, &ANavigationPoint::OnOverlap);
}

void ANavigationPoint::OnOverlap(UPrimitiveComponent * OverlappedComp, AActor * OtherActor, UPrimitiveComponent * OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult)
{
	if (OtherActor->ActorHasTag("Character"))
	{
		Collision->bGenerateOverlapEvents = false;
		UE_LOG(LogTemp, Log, TEXT("$$$$$$$$$$$$$$ %s"), *GetName());
		NaviEvent.ExecuteIfBound();
	}
}