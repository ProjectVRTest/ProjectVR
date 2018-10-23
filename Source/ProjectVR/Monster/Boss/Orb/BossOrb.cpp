// Fill out your copyright notice in the Description page of Project Settings.

#include "BossOrb.h"
#include "Particles/ParticleSystemComponent.h"
#include "Particles/ParticleSystem.h"
#include "UObject/ConstructorHelpers.h"
#include "Components/SphereComponent.h"
#include "BossOrbWave.h"
#include "TimerManager.h"
#include "Engine/World.h"
#include "Kismet/GameplayStatics.h"
#include "MyCharacter/MotionControllerCharacter.h"
#include "MyCharacter/CameraLocation.h"
#include "Monster/SwordWaveTarget.h"

// Sets default values
ABossOrb::ABossOrb()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	OrbParticleComponent = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("SwordWaveTailComponent"));
	SetRootComponent(OrbParticleComponent);

	static ConstructorHelpers::FObjectFinder<UParticleSystem>PT_Orb(TEXT("ParticleSystem'/Game/Assets/Effect/Orb/PS_Orb.PS_Orb'"));
	if (PT_Orb.Succeeded())
	{
		OrbParticle = PT_Orb.Object;
	}

	static ConstructorHelpers::FObjectFinder<UParticleSystem>PT_OrbExplosion(TEXT("ParticleSystem'/Game/Assets/Effect/ES_Skill/PS_GPP_MagicDark_Explosion.PS_GPP_MagicDark_Explosion'"));
	if (PT_OrbExplosion.Succeeded())
	{
		OrbExplosion = PT_OrbExplosion.Object;
	}

	OrbParticleComponent->Template = OrbParticle;

	Sphere = CreateDefaultSubobject<USphereComponent>(TEXT("Sphere"));
	Sphere->SetupAttachment(GetRootComponent());
	
	OrbWaveSpawn = CreateDefaultSubobject<USceneComponent>(TEXT("OrbWaveSpawn"));
	OrbWaveSpawn->SetupAttachment(GetRootComponent());
	OrbWaveSpawn->SetRelativeLocation(FVector(30.0f, 0, 0));
	OrbWaveMaxCount = 5;
}

// Called when the game starts or when spawned
void ABossOrb::BeginPlay()
{
	Super::BeginPlay();

	GetWorld()->GetTimerManager().SetTimer(OrbWaveFireTimer, this, &ABossOrb::FireWave, 3.0f,true,2.0f);
}

// Called every frame
void ABossOrb::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ABossOrb::FireWave()
{
	if (OrbWaveMaxCount > 0)
	{
		FActorSpawnParameters SpawnActorOption;
		SpawnActorOption.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

		ABossOrbWave* OrbWave = GetWorld()->SpawnActor<ABossOrbWave>(OrbWave->StaticClass(), OrbWaveSpawn->GetComponentLocation(), GetActorRotation(), SpawnActorOption);
		
		OrbWaveMaxCount--;

		if (OrbWave)
		{
			AMotionControllerCharacter* MyCharacter = Cast<AMotionControllerCharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));

			if (MyCharacter)
			{
				FVector LockonTargetLocation = MyCharacter->CameraLocation->GetActorLocation();

				ASwordWaveTarget* SwordWaveTarget = GetWorld()->SpawnActor<ASwordWaveTarget>(SwordWaveTarget->StaticClass(), LockonTargetLocation, FRotator::ZeroRotator);

				if (SwordWaveTarget)
				{
					OrbWave->Homing(SwordWaveTarget);
				}
			}
		}
	}

	if (OrbWaveMaxCount <= 0)
	{
		OrbWaveMaxCount = 0;
		GetWorld()->GetTimerManager().SetTimer(OrbExplosionTimer, this, &ABossOrb::OrbExplosionStart,1.0f,false);
	}
}

void ABossOrb::OrbExplosionStart()
{
	UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), OrbExplosion, GetActorLocation());
	Destroy();
}