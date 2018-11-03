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
#include "kismet/KismetMathLibrary.h"
#include "MyCharacter/MotionControllerCharacter.h"
#include "MyCharacter/CameraLocation.h"
#include "Monster/SwordWaveTarget.h"
#include "Monster/Boss/BossAIController.h"
#include "Monster/Boss/Boss.h"


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
	Sphere->SetCollisionProfileName("OverlapOnlyPawn");

	OrbWaveSpawn = CreateDefaultSubobject<USceneComponent>(TEXT("OrbWaveSpawn"));
	OrbWaveSpawn->SetupAttachment(GetRootComponent());
	OrbWaveSpawn->SetRelativeLocation(FVector(30.0f, 0, 0));
	OrbWaveMaxCount = 100;
}

// Called when the game starts or when spawned
void ABossOrb::BeginPlay()
{
	Super::BeginPlay();

	float RandomOrbAttackTime = FMath::RandRange(1.0f, 2.5f);
	
	GetWorld()->GetTimerManager().SetTimer(OrbWaveFireTimer, this, &ABossOrb::FireWave, RandomOrbAttackTime, true, RandomOrbAttackTime);
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
		AMotionControllerCharacter* MyCharacter = Cast<AMotionControllerCharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));

		if (MyCharacter)
		{

			FRotator LookAt =UKismetMathLibrary::FindLookAtRotation(GetActorLocation(), MyCharacter->CameraLocation->GetActorLocation());

			SetActorRotation(LookAt);

			FActorSpawnParameters SpawnActorOption;
			SpawnActorOption.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

			ABossOrbWave* OrbWave = GetWorld()->SpawnActor<ABossOrbWave>(OrbWave->StaticClass(), OrbWaveSpawn->GetComponentLocation(), GetActorRotation(), SpawnActorOption);

			OrbWaveMaxCount--;

			if (OrbWave)
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
		GetWorld()->GetTimerManager().ClearTimer(OrbWaveFireTimer);
		GetWorld()->GetTimerManager().SetTimer(OrbExplosionTimer, this, &ABossOrb::DefaultOrbExplosionStart, 1.0f, false);
	}
}

void ABossOrb::DefaultOrbExplosionStart()
{
	GLog->Log(FString::Printf(TEXT("오브 파괴됨")));
	UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), OrbExplosion, GetActorLocation());

	if (OrbOwner)
	{
		OrbOwner->OrbMaxCount++;
	}
	Destroy();
}

void ABossOrb::ColorOrbExplosionStart()
{
	UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), OrbExplosion, GetActorLocation());
	Destroy();
}

void ABossOrb::OwnerSave(AActor * _OrbOwner)
{
	if (_OrbOwner)
	{
		OrbOwner = Cast<ABoss>(_OrbOwner);
	}
}

void ABossOrb::RandomFireLocation(FVector &Location)
{
	float LeftRandLocation = FMath::FRandRange(-5.0f, 5.0f);
	float RightRandLocation = FMath::FRandRange(5.0f, 10.0f);

	Location.Y = Location.Y - LeftRandLocation * RightRandLocation;
	Location.X = Location.X - LeftRandLocation * RightRandLocation;
}

float ABossOrb::TakeDamage(float Damage, FDamageEvent const & DamageEvent, AController * EventInstigator, AActor * DamageCauser)
{
	Super::TakeDamage(Damage, DamageEvent, EventInstigator, DamageCauser);


	return Damage;
}
