// Fill out your copyright notice in the Description page of Project Settings.

#include "CameraLocationCharacter.h"
#include "Components/SphereComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/SkeletalMeshComponent.h"

// Sets default values
ACameraLocationCharacter::ACameraLocationCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
	GetCapsuleComponent()->SetCollisionProfileName("OverlapAll");
	GetCapsuleComponent()->bHiddenInGame = false;
	/*GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	GetCapsuleComponent()->SetCollisionObjectType(ECollisionChannel::ECC_GameTraceChannel3);
	GetCapsuleComponent()->SetCollisionResponseToChannel(ECollisionChannel::ECC_GameTraceChannel1, ECollisionResponse::ECR_Block);
	GetCapsuleComponent()->SetCollisionResponseToChannel(ECollisionChannel::ECC_GameTraceChannel2, ECollisionResponse::ECR_Overlap);*/

	GetMesh()->SetCollisionProfileName("OverlapAll");

	Sphere = CreateDefaultSubobject<USphereComponent>(TEXT("Sphere"));
	Sphere->SetupAttachment(GetRootComponent());
	Sphere->SetCollisionProfileName("OverlapAll");

	Sphere->ComponentTags.Add(FName(TEXT("CameraLocation")));
	Sphere->ComponentTags.Add(FName(TEXT("DisregardForLeftHand")));
	Sphere->ComponentTags.Add(FName(TEXT("DisregardForRightHand")));

	Tags.Add(FName(TEXT("DisregardForLeftHand")));
	Tags.Add(FName(TEXT("DisregardForRightHand")));
}

// Called when the game starts or when spawned
void ACameraLocationCharacter::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ACameraLocationCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void ACameraLocationCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

