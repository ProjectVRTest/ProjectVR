// Fill out your copyright notice in the Description page of Project Settings.

#include "NMWeaponSword.h"
#include "Components/StaticMeshComponent.h"
#include "UObject/ConstructorHelpers.h"
#include "Engine/StaticMesh.h"
#include "Components/CapsuleComponent.h"

// Sets default values
ANMWeaponSword::ANMWeaponSword()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	SwordMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("SwordMesh"));
	SetRootComponent(SwordMesh);
	SwordMesh->SetCollisionProfileName("OverlapAll");
	static ConstructorHelpers::FObjectFinder<UStaticMesh>SM_Sword(TEXT("StaticMesh'/Game/Assets/CharacterEquipment/Monster/NormalMonster/Mesh/Weapon/Sword/SM_NormalMonsterSword.SM_NormalMonsterSword'"));

	if (SM_Sword.Succeeded())
	{
		SwordMesh->SetStaticMesh(SM_Sword.Object);
	}
}

// Called when the game starts or when spawned
void ANMWeaponSword::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ANMWeaponSword::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

