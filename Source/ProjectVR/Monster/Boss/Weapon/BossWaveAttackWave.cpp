// Fill out your copyright notice in the Description page of Project Settings.

#include "BossWaveAttackWave.h"
#include "UObject/ConstructorHelpers.h"
#include "Particles/ParticleSystemComponent.h"
#include "Particles/ParticleSystem.h"
#include "MyCharacter/CameraLocation.h"
#include "kismet/GameplayStatics.h"
#include "Engine/World.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Components/SphereComponent.h"
#include "TimerManager.h"

ABossWaveAttackWave::ABossWaveAttackWave()
{
	static ConstructorHelpers::FObjectFinder<UParticleSystem>PT_WaveAttackWave(TEXT("ParticleSystem'/Game/Assets/Effect/ES_Skill/PT_BossWaveAttack.PT_BossWaveAttack'"));
	if (PT_WaveAttackWave.Succeeded())
	{
		OrbWaveParticle = PT_WaveAttackWave.Object;
	}

	static ConstructorHelpers::FObjectFinder<UParticleSystem>PT_WaveAttackWaveExplosion(TEXT("ParticleSystem'/Game/Assets/Effect/ES_Skill/PT_WaveAttackExplosion.PT_WaveAttackExplosion'"));
	if (PT_WaveAttackWaveExplosion.Succeeded())
	{
		OrbWaveExplosion = PT_WaveAttackWaveExplosion.Object;
	}

	OrbWaveParticleComponent->Template = nullptr;
	OrbWaveParticleComponent->SetRelativeLocation(FVector(1.0f, 1.0f, 1.0f));

	Projecttile->InitialSpeed = 800.0f;
	Projecttile->MaxSpeed = 800.0f;

	Sphere->ComponentTags.Add(FName(TEXT("BossWaveAttackWave")));
	Sphere->bHiddenInGame = false;
	Sphere->SetSphereRadius(80.0f);

	Tags.Add(FName(TEXT("DisregardForLeftHand")));
	Tags.Add(FName(TEXT("DisregardForRightHand")));
}

void ABossWaveAttackWave::BeginPlay()
{
	Super::BeginPlay();

	GetWorld()->GetTimerManager().SetTimer(GroundEffectSpawnTimer, this, &ABossWaveAttackWave::GroundEffectSpawn, 0.2f, true, 0.2f);
}

void ABossWaveAttackWave::BossOrbWaveBeginOverlap(UPrimitiveComponent * OverlappedComponent, AActor * OtherActor, UPrimitiveComponent * OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult)
{
	if (OtherComp->ComponentHasTag(TEXT("CameraLocation")))
	{
		GLog->Log(FString::Printf(TEXT("레드오브미사일 부딪힘")));

		ACameraLocation* CameraLocation = Cast<ACameraLocation>(OtherComp->GetOwner());

		if (CameraLocation)
		{
			UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), OrbWaveExplosion, OtherComp->GetComponentLocation());
			UGameplayStatics::ApplyDamage(OtherComp->GetOwner()->GetAttachParentActor(), 5.0f, nullptr, this, nullptr);
			GetWorld()->GetTimerManager().ClearTimer(GroundEffectSpawnTimer);
			Destroy();
		}
	}
	else if (OtherComp->ComponentHasTag(TEXT("BossWaveTarget")))
	{
		GLog->Log(FString::Printf(TEXT("보스 웨이브 타겟과 부딪힘")));
		UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), OrbWaveExplosion, OtherActor->GetActorLocation());
		Projecttile->bIsHomingProjectile = false;
		OtherActor->Destroy();
		GetWorld()->GetTimerManager().ClearTimer(GroundEffectSpawnTimer);
		Destroy();
	}
}

void ABossWaveAttackWave::GroundEffectSpawn()
{
	FVector TraceEnd = GetActorLocation() + (GetActorUpVector()*-10000.0f);
	TArray<TEnumAsByte<EObjectTypeQuery>>ObjectTypes;

	ObjectTypes.Add(UEngineTypes::ConvertToObjectType(ECollisionChannel::ECC_WorldStatic));

	TArray<AActor*>IgonreActors;
	IgonreActors.Add(this);

	FHitResult HitResult;

	bool CanSpawn = UKismetSystemLibrary::LineTraceSingleForObjects(
		GetWorld(),
		GetActorLocation(),
		TraceEnd,
		ObjectTypes,
		true,
		IgonreActors,
		EDrawDebugTrace::None,
		HitResult,
		true
	);

	if (CanSpawn)
	{
		UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), OrbWaveParticle, HitResult.Location);
	}
}