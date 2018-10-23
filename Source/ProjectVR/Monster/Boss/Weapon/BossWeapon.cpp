// Fill out your copyright notice in the Description page of Project Settings.

#include "BossWeapon.h"
#include "Components/StaticMeshComponent.h"
#include "Engine/StaticMesh.h"
#include "Components/CapsuleComponent.h"
#include "UObject/ConstructorHelpers.h"
#include "MyCharacter/MotionControllerCharacter.h"
#include "Monster/Boss/Boss.h"

// Sets default values
ABossWeapon::ABossWeapon()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	SwordMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("SwordMesh"));
	SetRootComponent(SwordMesh);
	SwordMesh->SetCollisionProfileName(TEXT("NoCollision"));

	static ConstructorHelpers::FObjectFinder<UStaticMesh>SM_Boss_Sword(TEXT("StaticMesh'/Game/Assets/CharacterEquipment/Monster/Boss/Weapon/SM_BossWeapon.SM_BossWeapon'"));

	if (SM_Boss_Sword.Succeeded())
	{
		SwordMesh->SetStaticMesh(SM_Boss_Sword.Object);
	}

	SwordCollision = CreateDefaultSubobject<UCapsuleComponent>(TEXT("SwordCollision"));
	SwordCollision->SetupAttachment(SwordMesh);
	SwordCollision->SetCollisionProfileName(TEXT("OverlapAll"));
	SwordCollision->SetRelativeLocation(FVector(31.0f, 0, 187.0f));
	SwordCollision->SetRelativeRotation(FRotator(77.0f, 0, 0));
	SwordCollision->SetRelativeScale3D(FVector(0.44f, 0.54f, 1.42f));

	SwordCollision->bHiddenInGame = false;

	IsWeaponAttack = false;
	IsParryingAttack = false;

	Tags.Add(FName(TEXT("BossWeapon")));
	Tags.Add(FName(TEXT("DisregardForRightHand")));
	Tags.Add(FName(TEXT("DisregardForLeftHand")));
}

// Called when the game starts or when spawned
void ABossWeapon::BeginPlay()
{
	Super::BeginPlay();
	
	if (SwordCollision)
	{
		SwordCollision->OnComponentBeginOverlap.AddDynamic(this, &ABossWeapon::WeaponBeginOverlap);
	}
}

// Called every frame
void ABossWeapon::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ABossWeapon::WeaponBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult)
{
	if (IsWeaponAttack)
	{
		if (OtherActor->ActorHasTag(TEXT("Character")))
		{
			IsWeaponAttack = false;

			AMotionControllerCharacter* MyCharacter = Cast<AMotionControllerCharacter>(OtherActor);

			if (MyCharacter)
			{
				ABoss* Boss = Cast<ABoss>(GetAttachParentActor());

				if (Boss)
				{

				}
			}
		}
	}
}
