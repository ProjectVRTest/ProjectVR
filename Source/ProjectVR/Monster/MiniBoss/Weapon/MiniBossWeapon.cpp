﻿// Fill out your copyright notice in the Description page of Project Settings.

#include "MiniBossWeapon.h"
#include "Components/StaticMeshComponent.h"	
#include "UObject/ConstructorHelpers.h"
#include "Components/CapsuleComponent.h"

#include "Engine/StaticMesh.h"
#include "kismet/GameplayStatics.h"
#include "MyCharacter/MotionControllerCharacter.h"
#include "Monster/MiniBoss/MiniBoss.h"

// Sets default values
AMiniBossWeapon::AMiniBossWeapon()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	SwordMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("SwordMesh"));
	SetRootComponent(SwordMesh);
	SwordMesh->SetCollisionProfileName(TEXT("OverlapOnlyPawn"));		
	
	static ConstructorHelpers::FObjectFinder<UStaticMesh>SM_Sword(TEXT("StaticMesh'/Game/Assets/Monster/MiniBoss/Weapon/Mesh/SM_MBWeapon.SM_MBWeapon'"));

	if (SM_Sword.Succeeded())
	{
		SwordMesh->SetStaticMesh(SM_Sword.Object);
	}

	SwordCollision = CreateDefaultSubobject<UCapsuleComponent>(TEXT("SwordCollision"));
	SwordCollision->SetupAttachment(SwordMesh);	
	SwordCollision->SetCollisionProfileName(TEXT("NoCollision"));

	SwordCollision->SetRelativeLocation(FVector(62.0f, 0, 0));
	SwordCollision->SetRelativeRotation(FRotator(90.0f, 0, 0));	
	SwordCollision->SetRelativeScale3D(FVector(0.7f, 0.7f, 2.3f));

	IsWeaponAttack = false;
	SwordCollision->bHiddenInGame = true;
	Tags.Add(FName(TEXT("MiniBossWeapon")));
	Tags.Add(FName(TEXT("DisregardForRightHand")));
	Tags.Add(FName(TEXT("DisregardForLeftHand")));
}

// Called when the game starts or when spawned
void AMiniBossWeapon::BeginPlay()
{
	Super::BeginPlay();
	
	if (SwordMesh)
	{
		SwordMesh->OnComponentBeginOverlap.AddDynamic(this, &AMiniBossWeapon::WeaponBeginOverlap);
	}
}

// Called every frame
void AMiniBossWeapon::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);	
}

void AMiniBossWeapon::WeaponBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult)
{
	if (IsWeaponAttack)
	{
		if (OtherActor->ActorHasTag(TEXT("Character")))
		{
			IsWeaponAttack = false;
			
			AMotionControllerCharacter* MyCharacter = Cast<AMotionControllerCharacter>(OtherActor);

			if (MyCharacter)
			{
				AMiniBoss* MiniBoss = Cast<AMiniBoss>(GetAttachParentActor());

				if (MiniBoss)
				{
					// (오버랩발생된 액터, 데미지, 데미지를가한 주체, 실제 데미지를 가한주체, 데미지종류클래스)
					//UGameplayStatics::ApplyDamage(OtherActor, 10.0f, nullptr, MiniBoss, nullptr);
				}				
			}
			//GLog->Log(FString::Printf(TEXT("캐릭터 때림")));
		}
	}	
}