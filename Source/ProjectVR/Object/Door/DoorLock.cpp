// Fill out your copyright notice in the Description page of Project Settings.

#include "DoorLock.h"
#include "Components/StaticMeshComponent.h"
#include "UObject/ConstructorHelpers.h"
#include "Engine/StaticMesh.h"
#include "Kismet/GameplayStatics.h"							// ĳ���� ã��
#include "MyCharacter/MotionControllerCharacter.h"

// Sets default values
ADoorLock::ADoorLock()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Lock = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Key"));
	Lock->SetupAttachment(RootComponent);

	static ConstructorHelpers::FObjectFinder<UStaticMesh>SM_Lock(TEXT("StaticMesh'/Game/Assets/CharacterEquipment/StarterContent/Shapes/Shape_Cube.Shape_Cube'"));		// ���۷��� ��η� ���� �Ž��� ã��
	if (SM_Lock.Succeeded())		// �� �޽��� ã���� ��� ����
	{
		Lock->SetStaticMesh(SM_Lock.Object);			// ����ƽ �޽��� �� ��� ����
	}

	Lock->SetEnableGravity(false);
	Lock->bGenerateOverlapEvents = true;
	Lock->SetCollisionProfileName("OverlapAll");
	Tags.Add("Lock");
	//Tags.Add(FName(TEXT("DisregardForLeftHand")));
	//Tags.Add(FName(TEXT("DisregardForRightHand")));
}

// Called when the game starts or when spawned
void ADoorLock::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ADoorLock::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ADoorLock::OnOverlap(UPrimitiveComponent * OverlappedComp, AActor * OtherActor, UPrimitiveComponent * OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult)
{
	if (OtherActor->ActorHasTag("Key"))
	{
		UE_LOG(LogTemp, Log, TEXT("Lopetegi"));
		OpenEvent.ExecuteIfBound();
	}
}

