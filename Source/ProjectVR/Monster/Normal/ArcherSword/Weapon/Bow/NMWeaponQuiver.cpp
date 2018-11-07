// Fill out your copyright notice in the Description page of Project Settings.

#include "NMWeaponQuiver.h"
#include "Components/StaticMeshComponent.h"
#include "Engine/StaticMesh.h"
#include "UObject/ConstructorHelpers.h"
#include "Monster/Normal/ArcherSword/NormalMonster.h"
#include "Monster/Normal/ArcherSword/Weapon/Bow/NMWeaponArrow.h"

// Sets default values
ANMWeaponQuiver::ANMWeaponQuiver()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	QuiverMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("QuiverMesh"));
	SetRootComponent(QuiverMesh);
	QuiverMesh->SetCollisionProfileName("NoCollision");

	NMArrowMeshOne = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("NMArrowMeshOne"));
	NMArrowMeshOne->SetupAttachment(GetRootComponent(), TEXT("Arrow1"));
	NMArrowMeshOne->SetCollisionProfileName("NoCollision");

	NMArrowMeshTwo = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("NMArrowMeshTwo"));
	NMArrowMeshTwo->SetupAttachment(GetRootComponent(), TEXT("Arrow2"));
	NMArrowMeshTwo->SetCollisionProfileName("NoCollision");

	NMArrowMeshThree = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("NMArrowMeshThree"));
	NMArrowMeshThree->SetupAttachment(GetRootComponent(), TEXT("Arrow3"));
	NMArrowMeshThree->SetCollisionProfileName("NoCollision");

	NMArrowMeshFour = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("NMArrowMeshFour"));
	NMArrowMeshFour->SetupAttachment(GetRootComponent(), TEXT("Arrow4"));
	NMArrowMeshFour->SetCollisionProfileName("NoCollision");

	NMArrowMeshFive = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("NMArrowMeshFive"));
	NMArrowMeshFive->SetupAttachment(GetRootComponent(), TEXT("Arrow5"));
	NMArrowMeshFive->SetCollisionProfileName("NoCollision");


	static ConstructorHelpers::FObjectFinder<UStaticMesh>SM_Quiver(TEXT("StaticMesh'/Game/Assets/CharacterEquipment/Monster/NormalMonster/Mesh/Weapon/Bow/Arrow/Mesh/SM_MoveArcherQuiver.SM_MoveArcherQuiver'"));

	if (SM_Quiver.Succeeded())
	{
		QuiverMesh->SetStaticMesh(SM_Quiver.Object);
	}

	static ConstructorHelpers::FObjectFinder<UStaticMesh>SM_NMArrow(TEXT("StaticMesh'/Game/Assets/CharacterEquipment/Monster/NormalMonster/Mesh/Weapon/Bow/Arrow/Mesh/SM_Arrow.SM_Arrow'"));

	if (SM_NMArrow.Succeeded())
	{
		NMArrow = SM_NMArrow.Object;
	}
}

// Called when the game starts or when spawned
void ANMWeaponQuiver::BeginPlay()
{
	Super::BeginPlay();
	
	ANormalMonster* NormalMonster = Cast<ANormalMonster>(GetOwner());


	if (NormalMonster)
	{
		ArrowCount = NormalMonster->ArrowCount;

		FActorSpawnParameters SpawnActorOption;
		SpawnActorOption.Owner = this;
		SpawnActorOption.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;


		for (int i = 0; i < ArrowCount; i++)
		{
			/*NMArrow = GetWorld()->SpawnActor<ANMWeaponArrow>(NMArrow->StaticClass(), SpawnActorOption);

			if (NMArrow)
			{
				ArrowPush(NMArrow);
			}*/
		}
	}	
}

// Called every frame
void ANMWeaponQuiver::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ANMWeaponQuiver::ArrowPop()
{

}

void ANMWeaponQuiver::ArrowPush(UStaticMesh* Arrow)
{
	FAttachmentTransformRules AttachRules(EAttachmentRule::SnapToTarget, EAttachmentRule::SnapToTarget, EAttachmentRule::KeepWorld, false);

	if (Arrow)
	{
		/*int32 index= NMArrows.Add(Arrow);

		int32 CurrentArrowCount = NMArrows.Num();

		switch (CurrentArrowCount)
		{
		case 1:
			Arrow->AttachToComponent(QuiverMesh, AttachRules, TEXT("Arrow1"));
			break;
		case 2:
			Arrow->AttachToComponent(QuiverMesh, AttachRules, TEXT("Arrow2"));
			break;
		case 3:
			Arrow->AttachToComponent(QuiverMesh, AttachRules, TEXT("Arrow3"));
			break;
		case 4:
			Arrow->AttachToComponent(QuiverMesh, AttachRules, TEXT("Arrow4"));
			break;
		case 5:
			Arrow->AttachToComponent(QuiverMesh, AttachRules, TEXT("Arrow5"));
			break;
		}*/
	}	
}

