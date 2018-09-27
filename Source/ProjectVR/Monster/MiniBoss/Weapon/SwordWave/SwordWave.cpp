// Fill out your copyright notice in the Description page of Project Settings.

#include "SwordWave.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Components/StaticMeshComponent.h"
#include "UObject/ConstructorHelpers.h"
#include "Engine/StaticMesh.h"
#include "MyCharacter/CameraLocation.h"
#include "Kismet/GameplayStatics.h"
#include "Particles/ParticleSystem.h"

// Sets default values
ASwordWave::ASwordWave()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	SetRootComponent(Mesh);
	Mesh->SetRelativeScale3D(FVector(0.4f, 0.4f, 0.4f));
	Mesh->SetCollisionProfileName("OverlapAll");
	static ConstructorHelpers::FObjectFinder<UStaticMesh>SM_SwordWave(TEXT("StaticMesh'/Game/Assets/CharacterEquipment/StarterContent/Shapes/Shape_Sphere.Shape_Sphere'"));

	if (SM_SwordWave.Succeeded())
	{
		Mesh->SetStaticMesh(SM_SwordWave.Object);
	}
	Projecttile = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("Projecttile"));
	Projecttile->InitialSpeed = 4000.0f;
	Projecttile->MaxSpeed = 4000.0f;

	static ConstructorHelpers::FObjectFinder<UParticleSystem>PT_SwordWaveExplosion(TEXT("ParticleSystem'/Game/Assets/CharacterEquipment/StarterContent/Particles/P_Explosion.P_Explosion'"));
	if (PT_SwordWaveExplosion.Succeeded())
	{
		SwordWaveExplosion = PT_SwordWaveExplosion.Object;
	}

}

// Called when the game starts or when spawned
void ASwordWave::BeginPlay()
{
	Super::BeginPlay();
	
	OnActorBeginOverlap.AddDynamic(this, &ASwordWave::SwordWaveBeginOverlap);
}

// Called every frame
void ASwordWave::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ASwordWave::Homing(ACameraLocation* Target)
{
	if (Target)
	{
		Projecttile->HomingTargetComponent = Target->GetRootComponent();
		Projecttile->bIsHomingProjectile = true;
		Projecttile->HomingAccelerationMagnitude = 30000.0f;
	}	
}

void ASwordWave::SwordWaveBeginOverlap(AActor* OverlappedActor, AActor* OtherActor)
{
	if (OtherActor)
	{
		ACameraLocation* CameraLocation = Cast<ACameraLocation>(OtherActor);
		if (CameraLocation)
		{
			UGameplayStatics::SpawnEmitterAtLocation(GetWorld(),SwordWaveExplosion, OtherActor->GetActorLocation());
			Destroy();
		}		
	}
}

