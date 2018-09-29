// Fill out your copyright notice in the Description page of Project Settings.

#include "NMWeaponBow.h"
#include "Components/SkeletalMeshComponent.h"
#include "UObject/ConstructorHelpers.h"
#include "Animation/AnimBlueprint.h"

// Sets default values
ANMWeaponBow::ANMWeaponBow()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	BowMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("BowMesh"));
	SetRootComponent(BowMesh);
	BowMesh->SetCollisionProfileName("OverlapAll");
	static ConstructorHelpers::FObjectFinder<USkeletalMesh>SKM_Bow(TEXT("SkeletalMesh'/Game/Assets/CharacterEquipment/Monster/NormalMonster/Mesh/Weapon/Bow/BowBody/Mesh/bow_anim.bow_anim'"));

	if (SKM_Bow.Succeeded())
	{
		BowMesh->SetSkeletalMesh(SKM_Bow.Object);
	}

	static ConstructorHelpers::FObjectFinder<UClass>ABP_Bow(TEXT("AnimBlueprint'/Game/Blueprints/Monster/Normal/Blueprints/Weapon/Bow/ABP_Bow.ABP_Bow_C'"));

	if (ABP_Bow.Succeeded())
	{
		UClass* BowAnimBlueprint = ABP_Bow.Object;

		if (BowAnimBlueprint)
		{
			BowMesh->SetAnimationMode(EAnimationMode::AnimationBlueprint);
			BowMesh->SetAnimInstanceClass(BowAnimBlueprint);
		}
	}
}

// Called when the game starts or when spawned
void ANMWeaponBow::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ANMWeaponBow::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

