// Fill out your copyright notice in the Description page of Project Settings.

#include "BossBlueOrb.h"
#include "UObject/ConstructorHelpers.h"
#include "Particles/ParticleSystem.h"
#include "Particles/ParticleSystemComponent.h"
#include "Monster/Boss/Orb/Ultimate/Wave/BossBlueOrbWave.h"
#include "TimerManager.h"
#include "Engine/World.h"
#include "Kismet/GameplayStatics.h"
#include "MyCharacter/MotionControllerCharacter.h"
#include "MyCharacter/CameraLocation.h"
#include "Monster/SwordWaveTarget.h"
#include "kismet/KismetMathLibrary.h"

ABossBlueOrb::ABossBlueOrb()
{
	PrimaryActorTick.bCanEverTick = true;
	static ConstructorHelpers::FObjectFinder<UParticleSystem>PT_BlueOrb(TEXT("ParticleSystem'/Game/Assets/Effect/Orb/PS_OrbBlue.PS_OrbBlue'"));
	if (PT_BlueOrb.Succeeded())
	{
		OrbParticle = PT_BlueOrb.Object;
		OrbParticleComponent->Template = OrbParticle;
	}

	static ConstructorHelpers::FObjectFinder<UParticleSystem>PT_BlueOrbExplosion(TEXT("ParticleSystem'/Game/Assets/Effect/ES_Skill/PS_GPP_RedOrbExplosion.PS_GPP_RedOrbExplosion'"));
	if (PT_BlueOrbExplosion.Succeeded())
	{
		OrbExplosion = PT_BlueOrbExplosion.Object;
	}

	OrbWaveMaxCount = 100;
}

void ABossBlueOrb::BeginPlay()
{
	Super::BeginPlay();
}

void ABossBlueOrb::FireWave()
{
	GLog->Log(FString::Printf(TEXT("BlueOrbFire")));
	if (OrbWaveMaxCount > 0)
	{
		AMotionControllerCharacter* MyCharacter = Cast<AMotionControllerCharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));

		if (MyCharacter)
		{

			FRotator LookAt = UKismetMathLibrary::FindLookAtRotation(GetActorLocation(), MyCharacter->CameraLocation->GetActorLocation());

			SetActorRotation(LookAt);

			FActorSpawnParameters SpawnActorOption;
			SpawnActorOption.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

			ABossBlueOrbWave* OrbWave = GetWorld()->SpawnActor<ABossBlueOrbWave>(OrbWave->StaticClass(), OrbWaveSpawn->GetComponentLocation(), GetActorRotation(), SpawnActorOption);

			OrbWaveMaxCount--;

			if (OrbWave)
			{
				FVector LockonTargetLocation = MyCharacter->CameraLocation->GetActorLocation();

				RandomFireLocation(LockonTargetLocation);
							
				ASwordWaveTarget* SwordWaveTarget = GetWorld()->SpawnActor<ASwordWaveTarget>(SwordWaveTarget->StaticClass(), LockonTargetLocation, FRotator::ZeroRotator, SpawnActorOption);

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
