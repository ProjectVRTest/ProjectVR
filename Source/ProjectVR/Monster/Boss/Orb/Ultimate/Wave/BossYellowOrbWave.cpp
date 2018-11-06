﻿// Fill out your copyright notice in the Description page of Project Settings.

#include "BossYellowOrbWave.h"
#include "UObject/ConstructorHelpers.h"
#include "Particles/ParticleSystemComponent.h"
#include "Particles/ParticleSystem.h"
#include "MyCharacter/CameraLocation.h"
#include "kismet/GameplayStatics.h"
#include "Engine/World.h"
#include "GameFramework/ProjectileMovementComponent.h"

ABossYellowOrbWave::ABossYellowOrbWave()
{
	static ConstructorHelpers::FObjectFinder<UParticleSystem>PT_YellowOrbWave(TEXT("ParticleSystem'/Game/Assets/Effect/ES_Skill/PS_GPP_YellowMagicball.PS_GPP_YellowMagicball'"));
	if (PT_YellowOrbWave.Succeeded())
	{
		OrbWaveParticle = PT_YellowOrbWave.Object;
	}

	static ConstructorHelpers::FObjectFinder<UParticleSystem>PT_YellowOrbWaveExplosion(TEXT("ParticleSystem'/Game/Assets/Effect/ES_Skill/PS_YellowOrbWaveExplosion.PS_YellowOrbWaveExplosion'"));
	if (PT_YellowOrbWaveExplosion.Succeeded())
	{
		OrbWaveExplosion = PT_YellowOrbWaveExplosion.Object;
	}
	OrbWaveParticleComponent->Template = OrbWaveParticle;

	Projecttile->InitialSpeed = 800.0f;
	Projecttile->MaxSpeed = 800.0f;

	Tags.Add(FName(TEXT("BossYellowOrbWave")));
	Tags.Add(FName(TEXT("DisregardForLeftHand")));
	Tags.Add(FName(TEXT("DisregardForRightHand")));
}

void ABossYellowOrbWave::BeginPlay()
{
	Super::BeginPlay();
}

void ABossYellowOrbWave::BossOrbWaveBeginOverlap(UPrimitiveComponent * OverlappedComponent, AActor * OtherActor, UPrimitiveComponent * OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult)
{
	if (OtherComp->ComponentHasTag(TEXT("CameraLocation")))
	{
		GLog->Log(FString::Printf(TEXT("옐로우오브미사일 부딪힘")));

		ACameraLocation* CameraLocation = Cast<ACameraLocation>(OtherComp->GetOwner());

		if (CameraLocation)
		{
			//UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), OrbWaveExplosion, OtherComp->GetComponentLocation());
			UGameplayStatics::ApplyDamage(OtherComp->GetOwner()->GetAttachParentActor(), 5.0f, nullptr, this, nullptr);
			Destroy();
		}
	}
	else if (OtherComp->ComponentHasTag(TEXT("SwordWaveTarget")))
	{
		//UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), OrbWaveExplosion, OtherActor->GetActorLocation());
		Projecttile->bIsHomingProjectile = false;
		OtherActor->Destroy();
	}
}