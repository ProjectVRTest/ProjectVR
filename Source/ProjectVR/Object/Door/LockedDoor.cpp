// Fill out your copyright notice in the Description page of Project Settings.

#include "LockedDoor.h"
#include "Components/StaticMeshComponent.h"
#include "UObject/ConstructorHelpers.h"					// ������ �������κ��� ������ ����� ������ �� ���
#include "MyCharacter/MotionControllerCharacter.h"	// �����հ� ��ȣ�ۿ��ϱ� ���ؼ� ĳ���Ϳ� RightGrab�� ���� bool������ �ξ���
#include "Kismet/KismetMathLibrary.h"
#include "Kismet/GameplayStatics.h"							// ĳ���� ã��
#include "Engine/World.h"
#include "Engine/StaticMesh.h"
#include"Components/BoxComponent.h"
#include"Components/StaticMeshComponent.h"
#include "Object/Door/DoorLock.h"
#include "EngineUtils.h"

// Sets default values
ALockedDoor::ALockedDoor()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Scene = CreateDefaultSubobject<USceneComponent>(TEXT("Scene"));
	Scene->SetupAttachment(RootComponent);

	DoorScene1 = CreateDefaultSubobject<USceneComponent>(TEXT("DoorScene1"));
	DoorScene1->SetupAttachment(Scene);

	DoorScene2 = CreateDefaultSubobject<USceneComponent>(TEXT("DoorScene2"));
	DoorScene2->SetupAttachment(Scene);

	Door1 = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Door1"));
	Door1->SetupAttachment(DoorScene1);

	Door2 = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Door2"));
	Door2->SetupAttachment(DoorScene2);

	static ConstructorHelpers::FObjectFinder<UStaticMesh>SM_Door(TEXT("StaticMesh'/Game/Assets/MapBuild/RoughMap/Bridge/mesh/bridge_door_bridge_door_01.bridge_door_bridge_door_01'"));		// ���۷��� ��η� ���� �Ž��� ã��
	if (SM_Door.Succeeded())		// �� �޽��� ã���� ��� ����
	{
		Door1->SetStaticMesh(SM_Door.Object);			// ����ƽ �޽��� �� ��� ����
		Door2->SetStaticMesh(SM_Door.Object);			// ����ƽ �޽��� �� ��� ����
	}

	DoorScene1->SetRelativeLocation(FVector(252.5, 0.0f, 0.0f));
	DoorScene2->SetRelativeLocation(FVector(-252.5, 0.0f, 0.0f));
	DoorScene2->SetRelativeScale3D(FVector(-1.0f, 1.0f, 1.0f));

	Door1->SetRelativeScale3D(FVector(4.0f, 4.0f, 4.0f));
	Door2->SetRelativeScale3D(FVector(4.0f, 4.0f, 4.0f));


	AutoRot = FRotator(Door1->RelativeRotation.Pitch, Door1->RelativeRotation.Yaw + 60.0f, Door1->RelativeRotation.Roll);
	DefaultYaw = Door1->GetComponentRotation().Yaw;

	//Collision->bGenerateOverlapEvents = false;

	Door1->SetCollisionProfileName("BlockAll");
	Door2->SetCollisionProfileName("BlockAll");

	Tags.Add(FName("LockedDoor"));
	Tags.Add(FName(TEXT("DisregardForLeftHand")));
	Tags.Add(FName(TEXT("DisregardForRightHand")));
}

// Called when the game starts or when spawned
void ALockedDoor::BeginPlay()
{
	Super::BeginPlay();
	// �̺�Ʈ ���
	for (TActorIterator<ADoorLock> ActorItr(GetWorld()); ActorItr; ++ActorItr)
	{
		ActorItr->OpenEvent.BindUObject(this, &ALockedDoor::OpenDoor);
	}

}

// Called every frame
void ALockedDoor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (bGetKey)
	{

		if (Door1->RelativeRotation.Yaw < 60.0f)
		{
			Door1->SetRelativeRotation(FMath::Lerp(Door1->RelativeRotation, AutoRot, 0.05f));
			Door2->SetRelativeRotation(FMath::Lerp(Door2->RelativeRotation, AutoRot, 0.05f));
		}
	}
}

void ALockedDoor::OpenDoor()
{
	bGetKey = true;
	Door1->SetCollisionProfileName("NoCollision");
	Door2->SetCollisionProfileName("NoCollision");
}

