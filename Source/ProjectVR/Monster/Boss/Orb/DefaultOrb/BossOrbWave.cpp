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
#include "Monster/Boss/Orb/Ultimate/Wave/BossRedOrbWave.h"
#include "Monster/SwordWaveTarget.h"

// Sets default values
ABossOrbWave::ABossOrbWave()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	Sphere = CreateDefaultSubobject<USphereComponent>(TEXT("Sphere"));
	SetRootComponent(Sphere);
	Sphere->SetCollisionProfileName("OverlapAll");
	Sphere->SetSphereRadius(28.0f);
	Sphere->bHiddenInGame = false;

	PrimaryActorTick.bCanEverTick = false;
	OrbWaveParticleComponent = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("SwordWaveTailComponent"));
	OrbWaveParticleComponent->SetupAttachment(GetRootComponent());
	OrbWaveParticleComponent->SetRelativeScale3D(FVector(0.8f, 0.8f, 0.8f));	
	
	Projecttile = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("Projecttile"));	
	Projecttile->InitialSpeed = 3000.0f;
	Projecttile->MaxSpeed = 3000.0f;
	Projecttile->ProjectileGravityScale = 0;
	Projecttile->bRotationFollowsVelocity = true;

	static ConstructorHelpers::FObjectFinder<UParticleSystem>PT_OrbWave(TEXT("ParticleSystem'/Game/Assets/Effect/ES_Skill/PS_GPP_Magicball.PS_GPP_Magicball'"));
	if (PT_OrbWave.Succeeded())
	{
		OrbWaveParticle = PT_OrbWave.Object;
	}

	static ConstructorHelpers::FObjectFinder<UParticleSystem>PT_OrbWaveExplosion(TEXT("ParticleSystem'/Game/Assets/Effect/ES_Skill/PS_OrbWaveExplosion.PS_OrbWaveExplosion'"));
	if (PT_OrbWaveExplosion.Succeeded())
	{
		OrbWaveExplosion = PT_OrbWaveExplosion.Object;
	}

	OrbWaveParticleComponent->Template = OrbWaveParticle;

	Sphere->ComponentTags.Add(FName(TEXT("BossOrbWave")));
	Tags.Add(FName(TEXT("DisregardForLeftHand")));
	Tags.Add(FName(TEXT("DisregardForRightHand")));
	Damage = 15.0f;

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
		GLog->Log(FString::Printf(TEXT("기본 오브 부딪힘")));
		ACameraLocation* CameraLocation = Cast<ACameraLocation>(OtherComp->GetOwner());

		if (CameraLocation)
		{
			//UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), OrbWaveExplosion, OtherComp->GetComponentLocation());
			UGameplayStatics::ApplyDamage(OtherComp->GetOwner()->GetAttachParentActor(), Damage, nullptr, this, nullptr);
			Destroy();
		}
	}
	else if (OtherComp->ComponentHasTag(TEXT("SwordWaveTarget")))
	{
		ASwordWaveTarget* SwordWaveTarget = Cast<ASwordWaveTarget>(OtherComp->GetOwner());

		if (SwordWaveTarget)
		{
			//UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), OrbWaveExplosion, SwordWaveTarget->GetActorLocation());
			Projecttile->bIsHomingProjectile = false;			
			SwordWaveTarget->Destroy();
		}		
	}
}

float ABossOrbWave::GetOrbDamage()
{
	return Damage;
}
