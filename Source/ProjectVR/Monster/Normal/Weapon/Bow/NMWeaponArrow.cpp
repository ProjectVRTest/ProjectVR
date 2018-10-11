// Fill out your copyright notice in the Description page of Project Settings.

#include "NMWeaponArrow.h"
#include "Components/StaticMeshComponent.h"
#include "Engine/StaticMesh.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "UObject/ConstructorHelpers.h"
#include "MyCharacter/CameraLocation.h"
#include "Components/BoxComponent.h"

// Sets default values
ANMWeaponArrow::ANMWeaponArrow()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	SetRootComponent(Mesh);
	Mesh->SetRelativeScale3D(FVector(2.0f, 3.0f, 2.0f));
	Mesh->SetCollisionProfileName("NoCollision");
	static ConstructorHelpers::FObjectFinder<UStaticMesh>SM_Arrow(TEXT("StaticMesh'/Game/Assets/CharacterEquipment/Monster/NormalMonster/Mesh/Weapon/Bow/Arrow/Mesh/SM_Arrow.SM_Arrow'"));

	if (SM_Arrow.Succeeded())
	{
		Mesh->SetStaticMesh(SM_Arrow.Object);
	}

	ArrowHit = CreateDefaultSubobject<UBoxComponent>(TEXT("ArrowHit"));
	ArrowHit->SetupAttachment(Mesh);
	ArrowHit->SetCollisionProfileName("OverlapAll");
	ArrowHit->SetRelativeLocation(FVector(44.0f, 0, 0));	
	ArrowHit->SetRelativeScale3D(FVector(1.46f, 0.04f, 0.018f));

	Projecttile = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("Projecttile"));
	Projecttile->InitialSpeed = 3000.0f;
	Projecttile->MaxSpeed = 3000.0f;	
	Projecttile->ProjectileGravityScale = 1.0f;

	InitialLifeSpan = 2.0f;
}

// Called when the game starts or when spawned
void ANMWeaponArrow::BeginPlay()
{
	Super::BeginPlay();
	
	ArrowHit->OnComponentBeginOverlap.AddDynamic(this, &ANMWeaponArrow::ArrowBeginOverlap);
}

// Called every frame
void ANMWeaponArrow::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}
void ANMWeaponArrow::ArrowBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult)
{
	if (OtherActor)
	{
		ACameraLocation* CameraLocation = Cast<ACameraLocation>(OtherActor);

		if (CameraLocation)
		{
			GLog->Log(FString::Printf(TEXT("화살 카메라에 부딪힘")));

			//카메라 위치(벡터)에 호밍 시작구현
		}
	}
}

void ANMWeaponArrow::Homing(AActor * Target)
{
	if (Target)
	{
		Projecttile->HomingTargetComponent = Target->GetRootComponent();
		Projecttile->bIsHomingProjectile = true;
		Projecttile->HomingAccelerationMagnitude = 30000.0f;
	}
}

