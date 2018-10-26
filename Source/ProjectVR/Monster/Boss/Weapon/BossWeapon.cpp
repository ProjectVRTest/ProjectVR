// Fill out your copyright notice in the Description page of Project Settings.

#include "BossWeapon.h"
#include "Components/StaticMeshComponent.h"
#include "Engine/StaticMesh.h"
#include "Components/CapsuleComponent.h"
#include "UObject/ConstructorHelpers.h"
#include "MyCharacter/MotionControllerCharacter.h"
#include "Monster/Boss/Boss.h"
#include "kismet/GameplayStatics.h"

// Sets default values
ABossWeapon::ABossWeapon()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	SwordMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("SwordMesh"));
	SetRootComponent(SwordMesh);
	SwordMesh->SetCollisionProfileName(TEXT("OverlapAll"));
	SwordMesh->ComponentTags.Add(FName(TEXT("BossWeapon")));

	static ConstructorHelpers::FObjectFinder<UStaticMesh>SM_Boss_Sword(TEXT("StaticMesh'/Game/Assets/CharacterEquipment/Monster/Boss/Weapon/Mesh/SM_BossWeapon.SM_BossWeapon'"));

	if (SM_Boss_Sword.Succeeded())
	{
		SwordMesh->SetStaticMesh(SM_Boss_Sword.Object);
	}

	IsWeaponAttack = false;
	IsParryingAttack = false;

	Tags.Add(FName(TEXT("DisregardForRightHand")));
	Tags.Add(FName(TEXT("DisregardForLeftHand")));
}

// Called when the game starts or when spawned
void ABossWeapon::BeginPlay()
{
	Super::BeginPlay();
	
	if (SwordMesh)
	{
		SwordMesh->OnComponentBeginOverlap.AddDynamic(this, &ABossWeapon::WeaponBeginOverlap);
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
		if (OtherComp->ComponentHasTag(TEXT("CameraLocation")))
		{
			IsWeaponAttack = false;

			AMotionControllerCharacter* MyCharacter = Cast<AMotionControllerCharacter>(OtherComp->GetOwner()->GetAttachParentActor());

			if (MyCharacter)
			{
				ABoss* Boss = Cast<ABoss>(GetAttachParentActor());

				if (Boss)
				{
					UGameplayStatics::ApplyDamage(MyCharacter, 10.0f, nullptr, nullptr, nullptr);
				}
			}			
			GLog->Log(FString::Printf(TEXT("캐릭터 때림")));
		}
	}
}

