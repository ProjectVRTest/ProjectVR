// Fill out your copyright notice in the Description page of Project Settings.

#include "MiniBossSwordWave.h"
#include "Components/BoxComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "UObject/ConstructorHelpers.h"
#include "Components/StaticMeshComponent.h"
#include "Engine/StaticMesh.h"
#include "Particles/ParticleSystemComponent.h"
#include "Particles/ParticleSystem.h"
#include "MyCharacter/CameraLocation.h"
#include "TimerManager.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetSystemLibrary.h"

// Sets default values
AMiniBossSwordWave::AMiniBossSwordWave()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	SwordBox = CreateDefaultSubobject<UBoxComponent>(TEXT("Box"));
	SetRootComponent(SwordBox);
	SwordBox->SetCollisionProfileName("OverlapAll");
	SwordBox->SetBoxExtent(FVector(40.0f, 60.0f, 32.0f));
	SwordBox->ComponentTags.Add(FName(TEXT("MiniBossSwordWave")));

	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	Mesh->SetupAttachment(SwordBox);
	Mesh->SetCollisionProfileName("NoCollision");
	Mesh->SetRelativeLocation(FVector(-117.0f, 0, 0));
	Mesh->SetRelativeRotation(FRotator(90.0f, 0, 0));
	Mesh->SetRelativeScale3D(FVector(0.5f, 2.5f, 3.0f));
	
	static ConstructorHelpers::FObjectFinder<UStaticMesh>SM_Mesh(TEXT("StaticMesh'/Game/Assets/CharacterEquipment/Monster/MiniBoss/Effect/AttackEffect/Sphere.Sphere'"));

	if (SM_Mesh.Succeeded())
	{
		Mesh->SetStaticMesh(SM_Mesh.Object);
	}

	SwordWaveTailComponent = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("SwordWaveTailComponent"));
	SwordWaveTailComponent->SetupAttachment(SwordBox);
	SwordWaveTailComponent->SetRelativeLocation(FVector(-105.0f, 0, 0));

	static ConstructorHelpers::FObjectFinder<UParticleSystem>PT_SwordWaveTail(TEXT("ParticleSystem'/Game/Assets/CharacterEquipment/Monster/MiniBoss/Effect/AttackEffect/NewParticleSystem1.NewParticleSystem1'"));

	if (PT_SwordWaveTail.Succeeded())
	{
		SwordWaveTail = PT_SwordWaveTail.Object;
	}

	static ConstructorHelpers::FObjectFinder<UParticleSystem>PT_SwordWaveExplosion(TEXT("ParticleSystem'/Game/Assets/CharacterEquipment/Monster/MiniBoss/Effect/SwordWave/P_SwordWaveEnd.P_SwordWaveEnd'"));
	if (PT_SwordWaveExplosion.Succeeded())
	{
		SwordWaveExplosion = PT_SwordWaveExplosion.Object;
	}
	
	static ConstructorHelpers::FObjectFinder<UParticleSystem>PT_SwordWaveGround(TEXT("ParticleSystem'/Game/Assets/CharacterEquipment/StarterContent/Particles/P_Steam_Lit.P_Steam_Lit'"));
	if (PT_SwordWaveGround.Succeeded())
	{
		SwordWaveGround = PT_SwordWaveGround.Object;
	}

	SwordWaveTailComponent->Template = SwordWaveTail;

	Projecttile = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("Projecttile"));
	Projecttile->InitialSpeed = 2500.0f;
	Projecttile->MaxSpeed = 2500.0f;
	Projecttile->ProjectileGravityScale = 0;
	Projecttile->bRotationFollowsVelocity = true;
	
	Damage = 15.0f;

	Tags.Add(FName(TEXT("DisregardForLeftHand")));
	Tags.Add(FName(TEXT("DisregardForRightHand")));
}

// Called when the game starts or when spawned
void AMiniBossSwordWave::BeginPlay()
{
	Super::BeginPlay();
	
	if (SwordBox)
	{
		SwordBox->OnComponentBeginOverlap.AddDynamic(this, &AMiniBossSwordWave::SwordWaveBeginOverlap);

		GetWorld()->GetTimerManager().SetTimer(FloorSmokeTimer, this, &AMiniBossSwordWave::FloorSmokeSpawn, 0.7f, true, 0.7f);
	}
}

// Called every frame
void AMiniBossSwordWave::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AMiniBossSwordWave::Homing(AActor * Target)
{
	if (Target)
	{
		Projecttile->HomingTargetComponent = Target->GetRootComponent();
		Projecttile->bIsHomingProjectile = true;
		Projecttile->HomingAccelerationMagnitude = 30000.0f;
	}
}

float AMiniBossSwordWave::GetDamage()
{
	return Damage;
}

void AMiniBossSwordWave::SetDamage(float NewDamage)
{
	Damage = NewDamage;
}

void AMiniBossSwordWave::SwordWaveBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult)
{
	if (OtherComp->ComponentHasTag(TEXT("CameraLocation")))
	{
		ACameraLocation* CameraLocation = Cast<ACameraLocation>(OtherComp->GetOwner());

		if (CameraLocation)
		{
			UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), SwordWaveExplosion, OtherComp->GetComponentLocation());
			UGameplayStatics::ApplyDamage(OtherComp->GetOwner()->GetAttachParentActor(), Damage, nullptr, this, nullptr);
			GetWorld()->GetTimerManager().ClearTimer(FloorSmokeTimer);
			Destroy();
		}
	}
	else if (OtherComp->ComponentHasTag(TEXT("SwordWaveTarget")))
	{
		Projecttile->bIsHomingProjectile = false;
		OtherActor->Destroy();
	}
}

void AMiniBossSwordWave::FloorSmokeSpawn()
{
	FVector TraceEnd = GetActorLocation() + (GetActorUpVector()*-10000.0f);
	TArray<TEnumAsByte<EObjectTypeQuery>>ObjectTypes;

	ObjectTypes.Add(UEngineTypes::ConvertToObjectType(ECollisionChannel::ECC_WorldStatic));

	TArray<AActor*>IgonreActors;
	IgonreActors.Add(this);

	FHitResult HitResult;

	bool CanSpawn = UKismetSystemLibrary::LineTraceSingleForObjects(
		GetWorld(),
		GetActorLocation(),
		TraceEnd,
		ObjectTypes,
		true,
		IgonreActors,
		EDrawDebugTrace::None,
		HitResult,
		true
	);

	if (CanSpawn)
	{
		UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), SwordWaveGround, HitResult.Location);
	}
}
