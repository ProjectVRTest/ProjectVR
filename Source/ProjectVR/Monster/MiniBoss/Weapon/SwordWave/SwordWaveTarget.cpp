// Fill out your copyright notice in the Description page of Project Settings.

#include "SwordWaveTarget.h"


// Sets default values
ASwordWaveTarget::ASwordWaveTarget()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	InitialLifeSpan = 4.0f;
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

