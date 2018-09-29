// Fill out your copyright notice in the Description page of Project Settings.

#include "NMWeaponArrow.h"
#include "Components/StaticMeshComponent.h"
#include "Engine/StaticMesh.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "UObject/ConstructorHelpers.h"

// Sets default values
ANMWeaponArrow::ANMWeaponArrow()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	SetRootComponent(Mesh);
	Mesh->SetRelativeScale3D(FVector(1.0f, 2.0f, 2.0f));
	Mesh->SetCollisionProfileName("OverlapAll");
	static ConstructorHelpers::FObjectFinder<UStaticMesh>SM_Arrow(TEXT("StaticMesh'/Game/Assets/CharacterEquipment/Monster/NormalMonster/Mesh/Weapon/Bow/Arrow/Mesh/SM_Arrow.SM_Arrow'"));

	if (SM_Arrow.Succeeded())
	{
		Mesh->SetStaticMesh(SM_Arrow.Object);
	}

	Projecttile = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("Projecttile"));
	Projecttile->InitialSpeed = 3000.0f;
	Projecttile->MaxSpeed = 3000.0f;	

}

// Called when the game starts or when spawned
void ANMWeaponArrow::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ANMWeaponArrow::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

