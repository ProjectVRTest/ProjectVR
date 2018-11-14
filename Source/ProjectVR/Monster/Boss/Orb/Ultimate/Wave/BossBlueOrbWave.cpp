// Fill out your copyright notice in the Description page of Project Settings.

#include "BossBlueOrbWave.h"
#include "UObject/ConstructorHelpers.h"
#include "Particles/ParticleSystemComponent.h"
#include "Particles/ParticleSystem.h"
#include "MyCharacter/CameraLocation.h"
#include "kismet/GameplayStatics.h"
#include "Engine/World.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Components/SphereComponent.h"

ABossBlueOrbWave::ABossBlueOrbWave()
{
	static ConstructorHelpers::FObjectFinder<UParticleSystem>PT_BlueOrbWave(TEXT("ParticleSystem'/Game/Assets/Effect/ES_Skill/PS_GPP_BlueMagicball.PS_GPP_BlueMagicball'"));
	if (PT_BlueOrbWave.Succeeded())
	{
		OrbWaveParticle = PT_BlueOrbWave.Object;
	}

	OrbWaveParticleComponent->Template = OrbWaveParticle;

	Projecttile->InitialSpeed = 800.0f;
	Projecttile->MaxSpeed = 800.0f;

	Sphere->ComponentTags.Add(FName(TEXT("BossBlueOrbWave")));
	
	Damage = 10.0f;

	Tags.Add(FName(TEXT("DisregardForLeftHand")));
	Tags.Add(FName(TEXT("DisregardForRightHand")));
}

void ABossBlueOrbWave::BeginPlay()
{
	Super::BeginPlay();
}

void ABossBlueOrbWave::BossOrbWaveBeginOverlap(UPrimitiveComponent * OverlappedComponent, AActor * OtherActor, UPrimitiveComponent * OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult)
{
	if (OtherComp->ComponentHasTag(TEXT("CameraLocation")))
	{
		GLog->Log(FString::Printf(TEXT("파랑오브미사일 부딪힘")));

		ACameraLocation* CameraLocation = Cast<ACameraLocation>(OtherComp->GetOwner());

		if (CameraLocation)
		{
			UGameplayStatics::ApplyDamage(OtherComp->GetOwner()->GetAttachParentActor(), 5.0f, nullptr, this, nullptr);
			Destroy();
		}
	}
	else if (OtherComp->ComponentHasTag(TEXT("SwordWaveTarget")))
	{
		Projecttile->bIsHomingProjectile = false;
		OtherActor->Destroy();
	}
}
