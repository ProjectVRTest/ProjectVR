// Fill out your copyright notice in the Description page of Project Settings.

#include "MonsterParryingPoint.h"
#include "Components/SceneComponent.h"
#include "Components/SphereComponent.h"
#include "Particles/ParticleSystem.h"
#include "Particles/ParticleSystemComponent.h"
#include "UObject/ConstructorHelpers.h"
#include "Equipment/PlayerSword.h"
#include "Monster/MiniBoss/MiniBoss.h"
#include "kismet/GameplayStatics.h"
#include "Engine/World.h"
#include "Monster/Boss/Boss.h"

// Sets default values
AMonsterParryingPoint::AMonsterParryingPoint()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	Sphere = CreateDefaultSubobject<USphereComponent>(TEXT("Sphere"));
	SetRootComponent(Sphere);	
	Sphere->SetCollisionProfileName("OverlapAll");	
	Sphere->SetSphereRadius(16.0f);

	ParryingPointEffectComponent = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("ParryingPointEffectComponent"));
	ParryingPointEffectComponent->SetupAttachment(Sphere);

	static ConstructorHelpers::FObjectFinder<UParticleSystem>PT_ParryingPoint(TEXT("ParticleSystem'/Game/Assets/CharacterEquipment/Monster/MiniBoss/Effect/ParryingPointEffect/ParryingEffectTwo.ParryingEffectTwo'"));
	if (PT_ParryingPoint.Succeeded())
	{
		ParryingPointEffect = PT_ParryingPoint.Object;
	}

	static ConstructorHelpers::FObjectFinder<UParticleSystem>PT_ParryingPointExplosionEffect(TEXT("ParticleSystem'/Game/Assets/Effect/ES_Skill/PT_ParryingPointExplosion.PT_ParryingPointExplosion'"));
	if (PT_ParryingPointExplosionEffect.Succeeded())
	{
		ParryingPointExplosionEffect = PT_ParryingPointExplosionEffect.Object;
	}

	ParryingPointEffectComponent->Template = ParryingPointEffect;

	Tags.Add(FName(TEXT("DisregardForLeftHand")));
	Tags.Add(FName(TEXT("DisregardForRightHand")));

	IsAttackMiniBossWeapon = false;
	IsAttackBossWeapon = false;
	InitialLifeSpan = 2.5f; //패링포인트 수명

}

// Called when the game starts or when spawned
void AMonsterParryingPoint::BeginPlay()
{
	Super::BeginPlay();
	
	if (Sphere)
	{
		Sphere->OnComponentBeginOverlap.AddDynamic(this, &AMonsterParryingPoint::ParryingPointBeginOverlap);
	}
}

// Called every frame
void AMonsterParryingPoint::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AMonsterParryingPoint::ParryingPointBeginOverlap(UPrimitiveComponent * OverlappedComponent, AActor * OtherActor, UPrimitiveComponent * OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult)
{
	if (OtherComp->ComponentHasTag(TEXT("PlayerSwordCollision")))
	{
		APlayerSword* PlayerSword = Cast<APlayerSword>(OtherComp->GetOwner());

		if (PlayerSword)
		{
			AMiniBoss* MiniBoss = Cast<AMiniBoss>(GetAttachParentActor());

			if (MiniBoss)
			{
				if (PlayerSword->SwordMoveVelocity.Size() >= 1000.0f)
				{
					if (!IsAttackMiniBossWeapon && MiniBoss)
					{
						if (MiniBoss->CurrentParryingState == EMiniBossParryingState::ParryingLoop)
						{
							IsAttackMiniBossWeapon = true;
							MiniBoss->ParryingPointCount++;
							UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), ParryingPointExplosionEffect, OtherComp->GetComponentLocation());
							Destroy();
						}
					}
				}
			}
			else
			{
				ABoss* Boss = Cast<ABoss>(GetAttachParentActor());

				if (Boss)
				{
					if (PlayerSword->SwordMoveVelocity.Size() >= 1000.0f)
					{
						if (!IsAttackBossWeapon && Boss)
						{
							if (Boss->CurrentParryingState == EBossParryingState::ParryingLoop)
							{
								IsAttackBossWeapon = true;
								Boss->ParryingPointCount++;
								UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), ParryingPointExplosionEffect, OtherComp->GetComponentLocation());
								Destroy();
							}
						}
					}
				}
			}
		}
	}
}

