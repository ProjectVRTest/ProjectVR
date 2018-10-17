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

// Sets default values
ABossAddAttackBall::ABossAddAttackBall()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	Mesh->SetCollisionProfileName("NoCollision");
	Mesh->SetRelativeScale3D(FVector(0.4f, 0.4f, 0.4f));
	SetRootComponent(Mesh);

	Sphere = CreateDefaultSubobject<USphereComponent>(TEXT("Sphere"));
	Sphere->SetupAttachment(Mesh);
	Sphere->SetCollisionProfileName("OverlapAll");
	Sphere->SetSphereRadius(55.0f);
	Sphere->SetRelativeLocation(FVector(0, 0, 48.0f));
	Sphere->bHiddenInGame = false;

	static ConstructorHelpers::FObjectFinder<UStaticMesh>SM_Mesh(TEXT("StaticMesh'/Game/Assets/CharacterEquipment/StarterContent/Shapes/Shape_Sphere.Shape_Sphere'"));

	if (SM_Mesh.Succeeded())
	{
		Mesh->SetStaticMesh(SM_Mesh.Object);
	}

	Projecttile = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("Projecttile"));
	Projecttile->InitialSpeed = 3000.0f;
	Projecttile->MaxSpeed = 3000.0f;
	Projecttile->ProjectileGravityScale = 0;
	
	Tags.Add(FName(TEXT("AttackBall")));
	Tags.Add(FName(TEXT("DisregardForLeftHand")));
	Tags.Add(FName(TEXT("DisregardForRightHand")));

	InitialLifeSpan = 5.0f;
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
	if (OtherActor->ActorHasTag(TEXT("CameraLocation")))
	{
		ACameraLocation* CameraLocation = Cast<ACameraLocation>(OtherActor);
		if (CameraLocation)
		{
			Destroy();
		}
	}
	else if (OtherActor->ActorHasTag(TEXT("BossWaveTarget")))
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
	else if (OtherActor->ActorHasTag(TEXT("SwordWaveTarget")))
	{
		Projecttile->HomingTargetComponent = nullptr;
	}
}

