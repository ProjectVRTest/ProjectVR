// Fill out your copyright notice in the Description page of Project Settings.

#include "SwordWave.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Components/StaticMeshComponent.h"
#include "UObject/ConstructorHelpers.h"
#include "Engine/StaticMesh.h"
#include "MyCharacter/CameraLocation.h"
#include "Kismet/GameplayStatics.h"
#include "Particles/ParticleSystem.h"
#include "Components/SphereComponent.h"
#include "Particles/ParticleSystemComponent.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Components/BoxComponent.h"

// Sets default values
ASwordWave::ASwordWave()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Sphere = CreateDefaultSubobject<USphereComponent>(TEXT("Sphere"));
	SetRootComponent(Sphere);
	Sphere->SetCollisionProfileName("NoCollision");

	Projecttile = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("Projecttile"));
	Projecttile->InitialSpeed = 3000.0f;
	Projecttile->MaxSpeed = 3000.0f;
	Projecttile->ProjectileGravityScale = 0;

	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	Mesh->SetupAttachment(Sphere);
	Mesh->SetCollisionProfileName("NoCollision");
	Mesh->SetRelativeRotation(FRotator(-90.0f, 0, 180.0f));
	Mesh->SetRelativeScale3D(FVector(0.5f, 2.5f, 5.0f));
	
	static ConstructorHelpers::FObjectFinder<UStaticMesh>SM_Mesh(TEXT("StaticMesh'/Game/Assets/CharacterEquipment/Monster/MiniBoss/Effect/AttackEffect/Sphere.Sphere'"));

	if (SM_Mesh.Succeeded())
	{
		Mesh->SetStaticMesh(SM_Mesh.Object);
	}

	static ConstructorHelpers::FObjectFinder<UParticleSystem>PT_SwordWaveExplosion(TEXT("ParticleSystem'/Game/Assets/CharacterEquipment/Monster/MiniBoss/Effect/SwordWave/P_SwordWaveEnd.P_SwordWaveEnd'"));
	if (PT_SwordWaveExplosion.Succeeded())
	{
		SwordWaveExplosion = PT_SwordWaveExplosion.Object;
	}

	SwordWaveTailComponent = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("SwordWaveTailComponent"));
	SwordWaveTailComponent->SetupAttachment(Sphere);

	static ConstructorHelpers::FObjectFinder<UParticleSystem>PT_SwordWaveTail(TEXT("ParticleSystem'/Game/Assets/CharacterEquipment/Monster/MiniBoss/Effect/AttackEffect/NewParticleSystem1.NewParticleSystem1'"));
	if (PT_SwordWaveTail.Succeeded())
	{
		SwordWaveTail = PT_SwordWaveTail.Object;
	}

	SwordWaveTailComponent->Template = SwordWaveTail;

	static ConstructorHelpers::FObjectFinder<UParticleSystem>PT_SwordWaveGround(TEXT("ParticleSystem'/Game/Assets/CharacterEquipment/StarterContent/Particles/P_Steam_Lit.P_Steam_Lit'"));
	if (PT_SwordWaveGround.Succeeded())
	{
		SwordWaveGround = PT_SwordWaveGround.Object;
	}

	SwordWaveHit = CreateDefaultSubobject<UBoxComponent>(TEXT("SwordWaveHit"));
	SwordWaveHit->SetupAttachment(Mesh);
	SwordWaveHit->SetCollisionProfileName("OverlapAll");
	SwordWaveHit->SetRelativeLocation(FVector(0, 0, -42.0f));
	SwordWaveHit->SetRelativeRotation(FRotator(90.0f, 0, 0));
	SwordWaveHit->SetRelativeScale3D(FVector(1.6f, 0.5f, 0.1f));

	Tags.Add(FName(TEXT("SwordWave")));
	Tags.Add(FName(TEXT("DisregardForLeftHand")));
	Tags.Add(FName(TEXT("DisregardForRightHand")));

	InitialLifeSpan = 4.0f; //검기 수명
}

// Called when the game starts or when spawned
void ASwordWave::BeginPlay()
{
	Super::BeginPlay();

	if (SwordWaveHit)
	{
		SwordWaveHit->OnComponentBeginOverlap.AddDynamic(this, &ASwordWave::SwordWaveBeginOverlap);
	}	
}

// Called every frame
void ASwordWave::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	FVector TraceEnd = GetActorLocation()+(GetActorUpVector()*-10000.0f);
	TArray<TEnumAsByte<EObjectTypeQuery>>ObjectTypes;

	ObjectTypes.Add(UEngineTypes::ConvertToObjectType(ECollisionChannel::ECC_WorldStatic));
	
	TArray<AActor*>IgonreActors;
	IgonreActors.Add(this);

	FHitResult HitResult;

	bool CanSpawn=UKismetSystemLibrary::LineTraceSingleForObjects(
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

void ASwordWave::Homing(AActor* Target)
{
	if (Target)
	{
		Projecttile->HomingTargetComponent = Target->GetRootComponent();
		Projecttile->bIsHomingProjectile = true;
		Projecttile->HomingAccelerationMagnitude = 30000.0f;
	}	
}

void ASwordWave::SwordWaveBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult)
{
	if (OtherComp->ComponentHasTag(TEXT("CameraLocation")))
	{
		ACameraLocation* CameraLocation = Cast<ACameraLocation>(OtherComp->GetOwner());

		if (CameraLocation)
		{
			UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), SwordWaveExplosion, OtherComp->GetComponentLocation());
			UGameplayStatics::ApplyDamage(OtherComp->GetOwner()->GetAttachParentActor(),Damage, nullptr, this, nullptr);
			Destroy();
		}
	}
	else if (OtherComp->ComponentHasTag(TEXT("SwordWaveTarget")))
	{
		Projecttile->bIsHomingProjectile = false;
		OtherActor->Destroy();
	}
}

void ASwordWave::SwordWaveRotatorModify(FRotator NewRotator)
{
	Mesh->SetRelativeRotation(NewRotator);
}

