// Fill out your copyright notice in the Description page of Project Settings.

#include "BossRedOrb.h"
#include "Particles/ParticleSystemComponent.h"
#include "Particles/ParticleSystem.h"
#include "UObject/ConstructorHelpers.h"
#include "Monster/Boss/Orb/Ultimate/Wave/BossRedOrbWave.h"
#include "TimerManager.h"
#include "Engine/World.h"
#include "Kismet/GameplayStatics.h"
#include "MyCharacter/MotionControllerCharacter.h"
#include "MyCharacter/CameraLocation.h"
#include "Monster/SwordWaveTarget.h"
#include "kismet/KismetMathLibrary.h"

ABossRedOrb::ABossRedOrb()
{
	static ConstructorHelpers::FObjectFinder<UParticleSystem>PT_RedOrb(TEXT("ParticleSystem'/Game/Assets/Effect/Orb/PS_OrbRed.PS_OrbRed'"));
	if (PT_RedOrb.Succeeded())
	{
		OrbParticle = PT_RedOrb.Object;
		OrbParticleComponent->Template = OrbParticle;
	}

	static ConstructorHelpers::FObjectFinder<UParticleSystem>PT_RedOrbExplosion(TEXT("ParticleSystem'/Game/Assets/Effect/ES_Skill/PS_GPP_RedOrbExplosion.PS_GPP_RedOrbExplosion'"));
	if (PT_RedOrbExplosion.Succeeded())
	{
		OrbExplosion = PT_RedOrbExplosion.Object;
	}
}

void ABossRedOrb::BeginPlay()
{
	Super::BeginPlay();

}

void ABossRedOrb::FireWave()
{
	GLog->Log(FString::Printf(TEXT("RedOrbFire")));
	if (OrbWaveMaxCount > 0)
	{
		AMotionControllerCharacter* MyCharacter = Cast<AMotionControllerCharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));

		if (MyCharacter)
		{

			FRotator LookAt = UKismetMathLibrary::FindLookAtRotation(GetActorLocation(), MyCharacter->CameraLocation->GetActorLocation());

			SetActorRotation(LookAt);

			FActorSpawnParameters SpawnActorOption;
			SpawnActorOption.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

			ABossRedOrbWave* OrbWave = GetWorld()->SpawnActor<ABossRedOrbWave>(OrbWave->StaticClass(), OrbWaveSpawn->GetComponentLocation(), GetActorRotation(), SpawnActorOption);

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