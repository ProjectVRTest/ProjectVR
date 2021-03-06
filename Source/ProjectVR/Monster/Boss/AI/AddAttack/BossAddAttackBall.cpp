// Fill out your copyright notice in the Description page of Project Settings.

#include "BossAddAttackBall.h"
#include "Components/SphereComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Engine/StaticMesh.h"
#include "UObject/ConstructorHelpers.h"
#include "MyCharacter/CameraLocation.h"
#include "Monster/SwordWaveTarget.h"
#include "Kismet/GameplayStatics.h"
#include "MyCharacter/MotionControllerCharacter.h"
#include "Particles/ParticleSystem.h"
#include "Particles/ParticleSystemComponent.h"

// Sets default values
ABossAddAttackBall::ABossAddAttackBall()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
	
	Sphere = CreateDefaultSubobject<USphereComponent>(TEXT("Sphere"));
	SetRootComponent(Sphere);	
	Sphere->SetCollisionProfileName("OverlapAll");
	Sphere->SetSphereRadius(30.0f);
	Sphere->bHiddenInGame = true;
	Sphere->ComponentTags.Add(FName(TEXT("AttackBall")));

	AddAttackBallEffectComponent = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("AddAttackBallEffectComponent"));
	AddAttackBallEffectComponent->SetupAttachment(Sphere);	

	Projecttile = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("Projecttile"));
	Projecttile->InitialSpeed = 2000.0f;
	Projecttile->MaxSpeed = 2000.0f;
	Projecttile->ProjectileGravityScale = 0;
	Projecttile->bRotationFollowsVelocity = true;
	
	static ConstructorHelpers::FObjectFinder<UParticleSystem>PT_AddAttackBallEffect(TEXT("ParticleSystem'/Game/Assets/Effect/ES_Skill/PT_BossAttackBall.PT_BossAttackBall'"));
	if (PT_AddAttackBallEffect.Succeeded())
	{
		AddAttackBallEffect = PT_AddAttackBallEffect.Object;
	}
	
	AddAttackBallEffectComponent->Template = AddAttackBallEffect;

	Damage = 20.0f;

	Tags.Add(FName(TEXT("DisregardForLeftHand")));
	Tags.Add(FName(TEXT("DisregardForRightHand")));

	InitialLifeSpan = 4.0f;
}

// Called when the game starts or when spawned
void ABossAddAttackBall::BeginPlay()
{
	Super::BeginPlay();
	
	if (Sphere)
	{
		Sphere->OnComponentBeginOverlap.AddDynamic(this, &ABossAddAttackBall::AttackBallBeginOverlap);
	}
}

// Called every frame
void ABossAddAttackBall::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ABossAddAttackBall::Homing(AActor * Target)
{
	if (Target)
	{
		Projecttile->HomingTargetComponent = Target->GetRootComponent();
		Projecttile->bIsHomingProjectile = true;
		Projecttile->HomingAccelerationMagnitude = 100000.0f;
	}
}

void ABossAddAttackBall::AttackBallBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult)
{
	if (OtherComp->ComponentHasTag(TEXT("CameraLocation")))
	{
		ACameraLocation* CameraLocation = Cast<ACameraLocation>(OtherComp->GetOwner());

		if (CameraLocation)
		{
			UGameplayStatics::ApplyDamage(OtherComp->GetOwner()->GetAttachParentActor(), Damage, nullptr, this, nullptr);
			Projecttile->bIsHomingProjectile = false;
			Destroy();
		}
	}
	else if (OtherComp->ComponentHasTag(TEXT("SwordWaveTarget")))
	{
		GLog->Log(FString::Printf(TEXT("웨이브 타겟 때림")));
		Projecttile->bIsHomingProjectile = false;
		OtherActor->Destroy();
	}
	else if (OtherComp->ComponentHasTag(TEXT("BossWaveTarget")))
	{
		AMotionControllerCharacter* MyCharacter = Cast<AMotionControllerCharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));

		if (MyCharacter)
		{
			FVector LockonTargetLocation = MyCharacter->CameraLocation->GetActorLocation();

			ASwordWaveTarget* SwordWaveTarget = GetWorld()->SpawnActor<ASwordWaveTarget>(SwordWaveTarget->StaticClass(), LockonTargetLocation, FRotator::ZeroRotator);

			if (SwordWaveTarget)
			{
				Homing(SwordWaveTarget);
			}
		}
	}
}

float ABossAddAttackBall::GetDamage()
{
	return Damage;
}

