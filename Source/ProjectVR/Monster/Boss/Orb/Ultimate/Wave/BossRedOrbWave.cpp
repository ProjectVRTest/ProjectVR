﻿// Fill out your copyright notice in the Description page of Project Settings.

#include "BossRedOrbWave.h"
#include "UObject/ConstructorHelpers.h"
#include "Particles/ParticleSystemComponent.h"
#include "Particles/ParticleSystem.h"
#include "MyCharacter/CameraLocation.h"
#include "kismet/GameplayStatics.h"
#include "Engine/World.h"
#include "GameFramework/ProjectileMovementComponent.h"


ABossRedOrbWave::ABossRedOrbWave()
{
	static ConstructorHelpers::FObjectFinder<UParticleSystem>PT_RedOrbWave(TEXT("ParticleSystem'/Game/Assets/Effect/ES_Skill/PS_GPP_RedMagicball.PS_GPP_RedMagicball'"));
	if (PT_RedOrbWave.Succeeded())
	{
		OrbWaveParticle = PT_RedOrbWave.Object;
	}

	OrbWaveParticleComponent->Template = OrbWaveParticle;

	Projecttile->InitialSpeed = 2500.0f;
	Projecttile->MaxSpeed = 2500.0f;

	Tags.Add(FName(TEXT("BossRedOrbWave")));
	Tags.Add(FName(TEXT("DisregardForLeftHand")));
	Tags.Add(FName(TEXT("DisregardForRightHand")));
}

void ABossRedOrbWave::BeginPlay()
{
	Super::BeginPlay();

}

void ABossRedOrbWave::BossOrbWaveBeginOverlap(UPrimitiveComponent * OverlappedComponent, AActor * OtherActor, UPrimitiveComponent * OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult)
{
	if (OtherComp->ComponentHasTag(TEXT("CameraLocation")))
	{
		GLog->Log(FString::Printf(TEXT("레드오브미사일 부딪힘")));

		ACameraLocation* CameraLocation = Cast<ACameraLocation>(OtherComp->GetOwner());

		if (CameraLocation)
		{
			UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), OrbWaveExplosion, OtherComp->GetComponentLocation());
			UGameplayStatics::ApplyDamage(OtherComp->GetOwner()->GetAttachParentActor(), 5.0f, nullptr, this, nullptr);
			Destroy();
		}
	}
	else if (OtherComp->ComponentHasTag(TEXT("SwordWaveTarget")))
	{
		UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), OrbWaveExplosion, OtherActor->GetActorLocation());
		Projecttile->bIsHomingProjectile = false;
		OtherActor->Destroy();
	}
}
