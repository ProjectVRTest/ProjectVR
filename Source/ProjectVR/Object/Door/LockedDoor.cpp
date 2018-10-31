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

	DoorMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("DoorMesh"));
	DoorMesh->SetupAttachment(Scene);

	//Collision = CreateDefaultSubobject<UBoxComponent>(TEXT("Collision"));
	//Collision->SetupAttachment(DoorMesh);

	static ConstructorHelpers::FObjectFinder<UStaticMesh>SM_Door(TEXT("StaticMesh'/Game/Assets/MapBuild/RoughMap/Bridge/mesh/bridge_door_bridge_door_01.bridge_door_bridge_door_01'"));		// ���۷��� ��η� ���� �Ž��� ã��
	if (SM_Door.Succeeded())		// �� �޽��� ã���� ��� ����
	{
		DoorMesh->SetStaticMesh(SM_Door.Object);			// ����ƽ �޽��� �� ��� ����
	}

	DoorMesh->SetRelativeScale3D(FVector(4.0f, 4.0f, 4.0f));

	//Collision->SetRelativeLocation(FVector(360.0f, 0.0f, -40.0f));
	//Collision->SetRelativeScale3D(FVector(10.0f, 0.7f, 2.6f));

	AutoRot = FRotator(DoorMesh->RelativeRotation.Pitch, DoorMesh->RelativeRotation.Yaw + 60.0f, DoorMesh->RelativeRotation.Roll);
	DefaultYaw = DoorMesh->GetComponentRotation().Yaw;

	//Collision->bGenerateOverlapEvents = false;

	DoorMesh->SetCollisionProfileName("BlockAll");
	//Collision->SetCollisionProfileName("BlockAll");

	Tags.Add(FName("LockedDoor"));
}

// Called when the game starts or when spawned
void ALockedDoor::BeginPlay()
{
	Super::BeginPlay();

	for (TActorIterator<ADoorLock> ActorItr(GetWorld()); ActorItr; ++ActorItr)
	{
		ActorItr->OpenEvent.BindUObject(this, &ALockedDoor::OpenDoor);
		UE_LOG(LogTemp, Log, TEXT("%s @@@@"), *ActorItr->GetName());
		/*if (ActorItr->GetName() == "MyActor1")
		{
		}*/
	}
	
}

// Called every frame
void ALockedDoor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (bGetKey)
	{
		if (DoorMesh->RelativeRotation.Yaw < 60.0f)
		{
			DoorMesh->SetRelativeRotation(FMath::Lerp(DoorMesh->RelativeRotation, AutoRot, 0.05f));
		}
	}
}

void ALockedDoor::OpenDoor()
{
	bGetKey = true;
	DoorMesh->SetCollisionProfileName("NoCollision");
}

