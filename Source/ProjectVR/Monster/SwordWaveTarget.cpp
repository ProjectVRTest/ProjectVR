// Fill out your copyright notice in the Description page of Project Settings.

#include "SwordWaveTarget.h"
#include "Components/BoxComponent.h"
#include "MiniBoss/Weapon/SwordWave/SwordWave.h"
#include "Normal/ArcherSword/Weapon/Bow/NMWeaponArrow.h"
#include "GameFramework/ProjectileMovementComponent.h"

// Sets default values
ASwordWaveTarget::ASwordWaveTarget()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	InitialLifeSpan = 4.0f;

	SwordWaveTargetBox = CreateDefaultSubobject<UBoxComponent>(TEXT("SwordWaveTargetBox"));
	SetRootComponent(SwordWaveTargetBox);
	SwordWaveTargetBox->SetCollisionProfileName("OverlapAll");
	SwordWaveTargetBox->SetRelativeScale3D(FVector(0.4f, 0.4f, 0.4f));
	SwordWaveTargetBox->bHiddenInGame = false;

	Tags.Add(FName(TEXT("SwordWaveTarget")));
	SwordWaveTargetBox->ComponentTags.Add(TEXT("DisregardForLeftHand"));
	SwordWaveTargetBox->ComponentTags.Add(TEXT("DisregardForRightHand"));	

	IsUseFlag = false;
}

// Called when the game starts or when spawned
void ASwordWaveTarget::BeginPlay()
{
	Super::BeginPlay();
	
	if (SwordWaveTargetBox)
	{
		SwordWaveTargetBox->OnComponentBeginOverlap.AddDynamic(this, &ASwordWaveTarget::SwordWaveTargetOverlap);
	}
}

// Called every frame
void ASwordWaveTarget::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ASwordWaveTarget::SwordWaveTargetOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult)
{
	if (!IsUseFlag)
	{
		if (OtherActor->ActorHasTag("SwordWave"))
		{
			ASwordWave* SwordWave = Cast<ASwordWave>(OtherActor);

			if (SwordWave)
			{
				SwordWave->Projecttile->HomingTargetComponent = nullptr;
			}
		}
		else if (OtherActor->ActorHasTag("NMArrow"))
		{
			ANMWeaponArrow* NMArrow = Cast<ANMWeaponArrow>(OtherActor);

			if (NMArrow)
			{
				NMArrow->Projecttile->HomingTargetComponent = nullptr;
			}
		}
	}
}
