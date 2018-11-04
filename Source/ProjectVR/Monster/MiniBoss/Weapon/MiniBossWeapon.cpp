// Fill out your copyright notice in the Description page of Project Settings.

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
	PrimaryActorTick.bCanEverTick = false;

	SwordMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("SwordMesh"));
	SetRootComponent(SwordMesh);
	SwordMesh->SetCollisionProfileName(TEXT("NoCollision"));		
	
	static ConstructorHelpers::FObjectFinder<UStaticMesh>SM_Sword(TEXT("StaticMesh'/Game/Assets/CharacterEquipment/Monster/MiniBoss/Weapon/Mesh/SM_MBWeapon.SM_MBWeapon'"));

	if (SM_Sword.Succeeded())
	{
		SwordMesh->SetStaticMesh(SM_Sword.Object);
	}

	static ConstructorHelpers::FObjectFinder<UMaterialInterface>M_Sword(TEXT("MaterialInstanceConstant'/Game/Assets/CharacterEquipment/Monster/MiniBoss/Weapon/Materials/M_MB_Weapon_Inst.M_MB_Weapon_Inst'"));

	if (M_Sword.Succeeded())
	{
		DefaultMaterials = M_Sword.Object;
		SwordMesh->SetMaterial(0,M_Sword.Object);
	}
	SwordCollision = CreateDefaultSubobject<UCapsuleComponent>(TEXT("SwordCollision"));
	SwordCollision->SetupAttachment(SwordMesh);	
	SwordCollision->SetCollisionProfileName(TEXT("OverlapAll"));

	SwordCollision->SetRelativeLocation(FVector(62.0f, 0, 0));
	SwordCollision->SetRelativeRotation(FRotator(90.0f, 0, 0));	
	SwordCollision->SetRelativeScale3D(FVector(1.3f, 1.3f, 2.5f));
	SwordCollision->bHiddenInGame = false;
	SwordCollision->ComponentTags.Add(FName(TEXT("MiniBossWeaponCollision")));
	IsWeaponAttack = false;
	IsParryingAttack = false;
		
	Tags.Add(FName(TEXT("DisregardForRightHand")));
	Tags.Add(FName(TEXT("DisregardForLeftHand")));
}

// Called when the game starts or when spawned
void AMiniBossWeapon::BeginPlay()
{
	Super::BeginPlay();
	
	if (SwordCollision)
	{
		SwordCollision->OnComponentBeginOverlap.AddDynamic(this, &AMiniBossWeapon::WeaponBeginOverlap);
	}
}

// Called every frame
void AMiniBossWeapon::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);	

	GLog->Log(FString::Printf(TEXT("칼 틱")));
}

void AMiniBossWeapon::WeaponBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult)
{
	if (IsWeaponAttack)
	{
		if (OtherComp->ComponentHasTag(TEXT("CameraLocation")))
		{
			IsWeaponAttack = false;
			
			AMotionControllerCharacter* MyCharacter = Cast<AMotionControllerCharacter>(OtherComp->GetOwner()->GetAttachParentActor());

			if (MyCharacter)
			{
				AMiniBoss* MiniBoss = Cast<AMiniBoss>(GetAttachParentActor());

				if (MiniBoss)
				{
					UGameplayStatics::ApplyDamage(OtherComp->GetOwner()->GetAttachParentActor(), 10.0f, nullptr, this, nullptr);
				}				
			}
			GLog->Log(FString::Printf(TEXT("캐릭터 때림")));
		}
	}	
}