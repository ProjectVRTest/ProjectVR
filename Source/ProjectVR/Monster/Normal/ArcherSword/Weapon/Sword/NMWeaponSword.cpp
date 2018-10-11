// Fill out your copyright notice in the Description page of Project Settings.

#include "NMWeaponSword.h"
#include "Components/StaticMeshComponent.h"
#include "UObject/ConstructorHelpers.h"
#include "Engine/StaticMesh.h"
#include "Components/CapsuleComponent.h"
#include "MyCharacter/MotionControllerCharacter.h"
#include "Monster/Normal/ArcherSword/NormalMonster.h"

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

	IsWeaponAttack = false;

	SwordCollision = CreateDefaultSubobject<UCapsuleComponent>(TEXT("SwordCollision"));
	SwordCollision->SetupAttachment(SwordMesh);
	SwordCollision->SetCollisionProfileName(TEXT("OverlapAll"));
	SwordCollision->SetRelativeScale3D(FVector(0.6f, 0.6f, 1.2f));
	IsWeaponAttack = false;

	SwordCollision->bHiddenInGame = false;

	Tags.Add(FName(TEXT("NormalMonsterSword")));
	Tags.Add(FName(TEXT("DisregardForRightHand")));
	Tags.Add(FName(TEXT("DisregardForLeftHand")));
}

// Called when the game starts or when spawned
void ANMWeaponSword::BeginPlay()
{
	Super::BeginPlay();
	
	if (SwordCollision)
	{
		SwordCollision->OnComponentBeginOverlap.AddDynamic(this, &ANMWeaponSword::SwordBeginOverlap);
	}
}

// Called every frame
void ANMWeaponSword::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ANMWeaponSword::SwordBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult)
{
	if (IsWeaponAttack)
	{
		if (OtherActor->ActorHasTag(TEXT("Character")))
		{
			IsWeaponAttack = false;

			AMotionControllerCharacter* MyCharacter = Cast<AMotionControllerCharacter>(OtherActor);

			if (MyCharacter)
			{
				ANormalMonster* NormalMonster = Cast<ANormalMonster>(GetAttachParentActor());

				if (NormalMonster)
				{
					// (오버랩발생된 액터, 데미지, 데미지를가한 주체, 실제 데미지를 가한주체, 데미지종류클래스)
					//UGameplayStatics::ApplyDamage(OtherActor, 10.0f, nullptr, MiniBoss, nullptr);
				}
			}
			GLog->Log(FString::Printf(TEXT("캐릭터 때림")));
		}
	}
}