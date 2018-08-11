﻿// Fill out your copyright notice in the Description page of Project Settings.

#include "MiniBossWeapon.h"
#include "Components/StaticMeshComponent.h"	
#include "UObject/ConstructorHelpers.h"
#include "Components/CapsuleComponent.h"

#include "Engine/StaticMesh.h"
// Sets default values
AMiniBossWeapon::AMiniBossWeapon()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	SwordMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("SwordMesh"));
	SetRootComponent(SwordMesh);
	SwordMesh->SetCollisionProfileName(TEXT("NoCollision"));		

	static ConstructorHelpers::FObjectFinder<UStaticMesh>SM_Sword(TEXT("StaticMesh'/Game/Assets/Monster/MiniBoss/Weapon/Mesh/SM_MBWeapon.SM_MBWeapon'"));

	if (SM_Sword.Succeeded())
	{
		SwordMesh->SetStaticMesh(SM_Sword.Object);
	}

	SwordCollision = CreateDefaultSubobject<UCapsuleComponent>(TEXT("SwordCollision"));
	SwordCollision->SetupAttachment(SwordMesh);	
	SwordCollision->SetCollisionProfileName(TEXT("OverlapOnlyPawn"));

	SwordCollision->SetRelativeLocation(FVector(62.0f, 0, 0));
	SwordCollision->SetRelativeRotation(FRotator(90.0f, 0, 0));	
	SwordCollision->SetRelativeScale3D(FVector(0.7f, 0.7f, 2.3f));
}

// Called when the game starts or when spawned
void AMiniBossWeapon::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AMiniBossWeapon::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}
