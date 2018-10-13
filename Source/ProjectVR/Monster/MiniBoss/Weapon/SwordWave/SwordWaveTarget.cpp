// Fill out your copyright notice in the Description page of Project Settings.

#include "SwordWaveTarget.h"
#include "Components/BoxComponent.h"

// Sets default values
ASwordWaveTarget::ASwordWaveTarget()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	InitialLifeSpan = 4.0f;

	SwordWaveTargetBox = CreateDefaultSubobject<UBoxComponent>(TEXT("SwordWaveTargetBox"));
	SetRootComponent(SwordWaveTargetBox);
	SwordWaveTargetBox->SetCollisionProfileName("OverlapAll");
	SwordWaveTargetBox->SetRelativeScale3D(FVector(0.4f, 0.4f, 0.4f));
	SwordWaveTargetBox->bHiddenInGame = false;

	Tags.Add(FName(TEXT("SwordWaveTarget")));
	SwordWaveTargetBox->ComponentTags.Add(TEXT("DisregardForLeftHand"));
	SwordWaveTargetBox->ComponentTags.Add(TEXT("DisregardForRightHand"));	
}

// Called when the game starts or when spawned
void ASwordWaveTarget::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ASwordWaveTarget::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

