// Fill out your copyright notice in the Description page of Project Settings.

#include "BossWaveTarget.h"
#include "Components/BoxComponent.h"

// Sets default values
ABossWaveTarget::ABossWaveTarget()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	BossWaveTargetBox = CreateDefaultSubobject<UBoxComponent>(TEXT("SwordWaveTargetBox"));
	SetRootComponent(BossWaveTargetBox);
	BossWaveTargetBox->SetCollisionProfileName("OverlapAll");
	BossWaveTargetBox->SetRelativeScale3D(FVector(0.4f, 0.4f, 0.4f));

	BossWaveTargetBox->bHiddenInGame = false;
	Tags.Add(FName(TEXT("BossWaveTarget")));
	Tags.Add(FName(TEXT("DisregardForLeftHand")));
	Tags.Add(FName(TEXT("DisregardForRightHand")));

	InitialLifeSpan = 3.0f;
}

// Called when the game starts or when spawned
void ABossWaveTarget::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void ABossWaveTarget::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}