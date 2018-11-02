// Fill out your copyright notice in the Description page of Project Settings.

#include "BossYellowOrb.h"
#include "UObject/ConstructorHelpers.h"
#include "Particles/ParticleSystem.h"
#include "Particles/ParticleSystemComponent.h"
#include "Monster/Boss/Orb/Ultimate/Wave/BossYellowOrbWave.h"
#include "TimerManager.h"
#include "Engine/World.h"
#include "Kismet/GameplayStatics.h"
#include "MyCharacter/MotionControllerCharacter.h"
#include "MyCharacter/CameraLocation.h"
#include "Monster/SwordWaveTarget.h"
#include "kismet/KismetMathLibrary.h"

ABossYellowOrb::ABossYellowOrb()
{
	static ConstructorHelpers::FObjectFinder<UParticleSystem>PT_YellowOrb(TEXT("ParticleSystem'/Game/Assets/Effect/Orb/PS_OrbYellow.PS_OrbYellow'"));
	if (PT_YellowOrb.Succeeded())
	{
		OrbParticle = PT_YellowOrb.Object;
		OrbParticleComponent->Template = OrbParticle;
	}

	static ConstructorHelpers::FObjectFinder<UParticleSystem>PT_YellowOrbExplosion(TEXT("ParticleSystem'/Game/Assets/Effect/ES_Skill/PS_GPP_YellowOrbExplosion.PS_GPP_YellowOrbExplosion'"));
	if (PT_YellowOrbExplosion.Succeeded())
	{
		OrbExplosion = PT_YellowOrbExplosion.Object;
	}
}

void ABossYellowOrb::BeginPlay()
{
	Super::BeginPlay();
}


void ABossYellowOrb::FireWave()
{
	GLog->Log(FString::Printf(TEXT("YellowOrbFire")));
	if (OrbWaveMaxCount > 0)
	{
		AMotionControllerCharacter* MyCharacter = Cast<AMotionControllerCharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));

		if (MyCharacter)
		{

			FRotator LookAt = UKismetMathLibrary::FindLookAtRotation(GetActorLocation(), MyCharacter->CameraLocation->GetActorLocation());

			SetActorRotation(LookAt);

			FActorSpawnParameters SpawnActorOption;
			SpawnActorOption.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

			ABossYellowOrbWave* OrbWave = GetWorld()->SpawnActor<ABossYellowOrbWave>(OrbWave->StaticClass(), OrbWaveSpawn->GetComponentLocation(), GetActorRotation(), SpawnActorOption);

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
		GetWorld()->GetTimerManager().SetTimer(OrbExplosionTimer, this, &ABossOrb::ColorOrbExplosionStart, 1.0f, false);
	}
}
