// Fill out your copyright notice in the Description page of Project Settings.

#include "SwordWaveTarget.h"
#include "Components/BoxComponent.h"
#include "Normal/ArcherSword/Weapon/Bow/NMWeaponArrow.h"
#include "Monster/Boss/AI/AddAttack/BossAddAttackBall.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "MyCharacter/MotionControllerCharacter.h"
#include "Kismet/GameplayStatics.h"
#include "Engine/World.h"
#include "MyCharacter/CameraLocation.h"

// Sets default values
ASwordWaveTarget::ASwordWaveTarget()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	SwordWaveTargetBox = CreateDefaultSubobject<UBoxComponent>(TEXT("SwordWaveTargetBox"));
	SetRootComponent(SwordWaveTargetBox);
	SwordWaveTargetBox->SetCollisionProfileName("OverlapAll");
	SwordWaveTargetBox->SetRelativeScale3D(FVector(0.4f, 0.4f, 0.4f));
	SwordWaveTargetBox->bHiddenInGame = false;

	SwordWaveTargetBox->ComponentTags.Add(FName(TEXT("SwordWaveTarget")));
	SwordWaveTargetBox->ComponentTags.Add(FName(TEXT("DisregardForLeftHand")));
	SwordWaveTargetBox->ComponentTags.Add(FName(TEXT("DisregardForRightHand")));

	InitialLifeSpan = 3.0f;
}

// Called when the game starts or when spawned
void ASwordWaveTarget::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void ASwordWaveTarget::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}