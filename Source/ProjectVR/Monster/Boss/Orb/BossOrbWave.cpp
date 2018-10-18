// Fill out your copyright notice in the Description page of Project Settings.

#include "BossOrbWave.h"
#include "Components/StaticMeshComponent.h"
#include "Engine/StaticMesh.h"
#include "Components/SphereComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "UObject/ConstructorHelpers.h"
#include "MyCharacter/CameraLocation.h"
#include "Particles/ParticleSystemComponent.h"
#include "Particles/ParticleSystem.h"
#include "kismet/GameplayStatics.h"

// Sets default values
ABossOrbWave::ABossOrbWave()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
	OrbWaveParticleComponent = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("SwordWaveTailComponent"));
	SetRootComponent(OrbWaveParticleComponent);
	OrbWaveParticleComponent->SetRelativeScale3D(FVector(0.8f, 0.8f, 0.8f));

	Sphere = CreateDefaultSubobject<USphereComponent>(TEXT("Sphere"));
	Sphere->SetupAttachment(GetRootComponent());
	Sphere->SetCollisionProfileName("OverlapAll");
	Sphere->SetSphereRadius(28.0f);
	Sphere->SetRelativeLocation(FVector(0, 0, 0.35f));
	Sphere->bHiddenInGame = false;
	
	Projecttile = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("Projecttile"));	
	Projecttile->InitialSpeed = 3000.0f;
	Projecttile->MaxSpeed = 3000.0f;
	Projecttile->ProjectileGravityScale = 0;
	
	static ConstructorHelpers::FObjectFinder<UParticleSystem>PT_OrbWave(TEXT("ParticleSystem'/Game/Assets/Effect/ES_Skill/PS_GPP_Magicball.PS_GPP_Magicball'"));
	if (PT_OrbWave.Succeeded())
	{
		OrbWaveParticle = PT_OrbWave.Object;
	}

	static ConstructorHelpers::FObjectFinder<UParticleSystem>PT_OrbWaveExplosion(TEXT("ParticleSystem'/Game/Assets/Effect/ES_Skill/PS_GPP_Magicball_Explosion.PS_GPP_Magicball_Explosion'"));
	if (PT_OrbWaveExplosion.Succeeded())
	{
		OrbWaveExplosion = PT_OrbWaveExplosion.Object;
	}

	OrbWaveParticleComponent->Template = OrbWaveParticle;

	Tags.Add(FName(TEXT("BossOrbWave")));
	Tags.Add(FName(TEXT("DisregardForLeftHand")));
	Tags.Add(FName(TEXT("DisregardForRightHand")));

	InitialLifeSpan = 5.0f;
}

// Called when the game starts or when spawned
void ABossOrbWave::BeginPlay()
{
	Super::BeginPlay();
	
	if (Sphere)
	{
		Sphere->OnComponentBeginOverlap.AddDynamic(this, &ABossOrbWave::BossOrbWaveBeginOverlap);
	}
}

// Called every frame
void ABossOrbWave::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);


}

void ABossOrbWave::Homing(AActor * Target)
{
	if (Target)
	{
		Projecttile->HomingTargetComponent = Target->GetRootComponent();
		Projecttile->bIsHomingProjectile = true;
		Projecttile->HomingAccelerationMagnitude = 100000.0f;
	}
}

void ABossOrbWave::BossOrbWaveBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult)
{
	if (OtherComp->ComponentHasTag(TEXT("CameraLocation")))
	{
		UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), OrbWaveExplosion, OtherComp->GetOwner()->GetActorLocation());
		Destroy();
	}

	if (OtherActor->ActorHasTag(TEXT("SwordWaveTarget")))
	{
		Sphere->SetCollisionProfileName("NoCollision");
		Projecttile->HomingTargetComponent = nullptr;
	}
}