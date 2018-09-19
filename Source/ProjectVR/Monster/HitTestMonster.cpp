// Fill out your copyright notice in the Description page of Project Settings.

#include "HitTestMonster.h"
#include "UObject/ConstructorHelpers.h"
#include "Components/SkeletalMeshComponent.h"			// For GetWorld
#include "Kismet/GameplayStatics.h"						// For Player
#include "MyCharacter/MotionControllerCharacter.h"		// For Player
#include "Kismet/KismetMathLibrary.h"
#include "Components/CapsuleComponent.h"

// Sets default values
AHitTestMonster::AHitTestMonster()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	static ConstructorHelpers::FObjectFinder<USkeletalMesh>MonsterMesh(TEXT("SkeletalMesh'/Game/Assets/CharacterEquipment/Monster/Dog/Mesh/MON_DOG_MESH.MON_DOG_MESH'"));
	if (MonsterMesh.Succeeded())
	{
		GetMesh()->SetSkeletalMesh(MonsterMesh.Object);
	}
	GetMesh()->SetRelativeLocation(FVector(-20.0f, 0.0f, -55.0f));
	GetMesh()->SetRelativeRotation(FRotator(0.0f, -90.0f, 0.0f));

	GetMesh()->SetCollisionProfileName("OverlapAll");
	GetCapsuleComponent()->SetCollisionProfileName("OverlapOnlyPawn");
	Tags.Add("TestMonster");
}

// Called when the game starts or when spawned
void AHitTestMonster::BeginPlay()
{
	Super::BeginPlay();

	GetMesh()->OnComponentBeginOverlap.AddDynamic(this, &AHitTestMonster::OnOverlap);
	
}

// Called every frame
void AHitTestMonster::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void AHitTestMonster::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

float AHitTestMonster::TakeDamage(float Damage, FDamageEvent const & DamageEvent, AController * EventInstigator, AActor * DamageCauser)
{
	Super::TakeDamage(Damage, DamageEvent, EventInstigator, DamageCauser);
	return 0.0f;
}

void AHitTestMonster::OnOverlap(UPrimitiveComponent * OverlappedComp, AActor * OtherActor, UPrimitiveComponent * OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult)
{
	if (OtherActor->ActorHasTag("Character"))
	{
		AMotionControllerCharacter* Character = Cast<AMotionControllerCharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));

		if (Character)
		{
			UGameplayStatics::ApplyDamage(OtherActor, 10.0f, UGameplayStatics::GetPlayerController(GetWorld(), 0), this, nullptr);		// 오버랩된 액터에 데미지 전달
			UE_LOG(LogTemp, Log, TEXT("Take Damage $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$"));

		}
	}
}

