// Fill out your copyright notice in the Description page of Project Settings.

#include "DontMoveArcher.h"


// Sets default values
ADontMoveArcher::ADontMoveArcher()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ADontMoveArcher::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ADontMoveArcher::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void ADontMoveArcher::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

