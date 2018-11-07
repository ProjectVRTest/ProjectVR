// Fill out your copyright notice in the Description page of Project Settings.

#include "LockedDoor.h"
#include "Components/StaticMeshComponent.h"
#include "UObject/ConstructorHelpers.h"					// 콘텐츠 브라우저로부터 포션의 모양을 가져올 때 사용
#include "MyCharacter/MotionControllerCharacter.h"	// 오른손과 상호작용하기 위해서 캐릭터에 RightGrab에 대한 bool변수를 두었읆
#include "Kismet/KismetMathLibrary.h"
#include "Kismet/GameplayStatics.h"							// 캐릭터 찾기
#include "Engine/World.h"
#include "Engine/StaticMesh.h"
#include"Components/BoxComponent.h"
#include"Components/StaticMeshComponent.h"
#include "Object/Door/DoorLock.h"
#include "EngineUtils.h"

#include "Object/Door/LockKey.h"		// 문열 때 키 생성
#include "Public/TimerManager.h" 
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

	static ConstructorHelpers::FObjectFinder<UStaticMesh>SM_Door(TEXT("StaticMesh'/Game/Assets/MapBuild/RoughMap/Bridge/mesh/bridge_door_bridge_door_01.bridge_door_bridge_door_01'"));
	if (SM_Door.Succeeded())
	{
		Door1->SetStaticMesh(SM_Door.Object);
		Door2->SetStaticMesh(SM_Door.Object);
	}

	Scene2 = CreateDefaultSubobject<USceneComponent>(TEXT("Scene2"));
	Scene2->SetupAttachment(RootComponent);

	LockScene = CreateDefaultSubobject<USceneComponent>(TEXT("LockScene"));
	LockScene->SetupAttachment(Scene2);

	CollisionScene = CreateDefaultSubobject<USceneComponent>(TEXT("CollisionScene"));
	CollisionScene->SetupAttachment(Scene2);

	KeySocket = CreateDefaultSubobject<USceneComponent>(TEXT("KeySocket"));
	KeySocket->SetupAttachment(Scene2);

	Body = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Body"));
	Body->SetupAttachment(LockScene);
	Button = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Button"));
	Button->SetupAttachment(LockScene);
	Opener = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Opener"));
	Opener->SetupAttachment(LockScene);
	Chain = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Chain"));
	Chain->SetupAttachment(LockScene);

	BoxCollision = CreateDefaultSubobject<UBoxComponent>(TEXT("BoxCollision"));
	BoxCollision->SetupAttachment(CollisionScene);

	static ConstructorHelpers::FObjectFinder<UStaticMesh>SM_Body(TEXT("StaticMesh'/Game/Assets/MapBuild/RoughMap/Locks/Lock/lock_low_Object002.lock_low_Object002'"));
	if (SM_Body.Succeeded())
	{
		Body->SetStaticMesh(SM_Body.Object);
	}
	static ConstructorHelpers::FObjectFinder<UStaticMesh>SM_Button(TEXT("StaticMesh'/Game/Assets/MapBuild/RoughMap/Locks/Lock/lock_low_Plane001.lock_low_Plane001'"));
	if (SM_Button.Succeeded())
	{
		Button->SetStaticMesh(SM_Button.Object);
	}
	static ConstructorHelpers::FObjectFinder<UStaticMesh>SM_Opener(TEXT("StaticMesh'/Game/Assets/MapBuild/RoughMap/Locks/Lock/lock_low_Object005.lock_low_Object005'"));
	if (SM_Opener.Succeeded())
	{
		Opener->SetStaticMesh(SM_Opener.Object);
	}
	static ConstructorHelpers::FObjectFinder<UStaticMesh>SM_Chain(TEXT("StaticMesh'/Game/Assets/MapBuild/RoughMap/Locks/Chain/chain.chain'"));
	if (SM_Chain.Succeeded())
	{
		Chain->SetStaticMesh(SM_Chain.Object);
	}


	Chain->SetRelativeLocation(FVector(-2.0f, -218.0f, 127.0f));
	Chain->SetWorldRotation(FRotator(0.0f, 0.0f, -57.0f));
	Chain->SetRelativeScale3D(FVector(7.5f, 7.5f, 7.5f));

	LockScene->SetRelativeRotation(FRotator(0.0f, 0.0f, 54.0f));
	LockScene->SetRelativeScale3D(FVector(0.3f, 0.3f, 0.3f));

	CollisionScene->SetRelativeLocation(FVector(0.0f, 90.0f, 22.0f));
	CollisionScene->SetRelativeScale3D(FVector(7.5f, 4.0f, 3.0f));

	KeySocket->SetRelativeLocation(FVector(0.32f, 22.6f, 14.17f));
	KeySocket->SetRelativeRotation(FRotator(0.0f, 0.0f, 54.0f));

	Body->SetEnableGravity(false);
	Body->bGenerateOverlapEvents = true;

	Body->SetCollisionProfileName("OverlapAll");
	Button->SetCollisionProfileName("OverlapAll");
	Chain->SetCollisionProfileName("OverlapAll");

	Scene2->SetRelativeLocation(FVector(-378.07f, 72.72f, 216.87f));
	Scene2->SetRelativeScale3D(FVector(3.0f, 3.0f, 3.0f));

	Door1->SetRelativeScale3D(FVector(4.0f, 4.0f, 4.0f));
	Door2->SetRelativeScale3D(FVector(4.0f, 4.0f, 4.0f));

	DoorScene1->SetRelativeLocation(FVector(252.5, 0.0f, 0.0f));
	DoorScene2->SetRelativeLocation(FVector(-1027.87, 0.0f, 0.0f));
	DoorScene2->SetRelativeScale3D(FVector(-1.0f, 1.0f, 1.0f));

	Scene->SetRelativeScale3D(FVector(0.5f, 0.5f, 0.5f));

	AutoRot = FRotator(Door1->RelativeRotation.Pitch, Door1->RelativeRotation.Yaw - 60.0f, Door1->RelativeRotation.Roll);
	DefaultYaw = Door1->GetComponentRotation().Yaw;

	//Collision->bGenerateOverlapEvents = false;

	Door1->SetCollisionProfileName("BlockAll");
	Door2->SetCollisionProfileName("BlockAll");

	BoxCollision->ComponentTags.Add("LockArea");
	Body->ComponentTags.Add("BodyLock");
	Scene2->ComponentTags.Add("Lock");
	Tags.Add(FName("LockedDoor"));
	Tags.Add(FName(TEXT("DisregardForLeftHand")));
	Tags.Add(FName(TEXT("DisregardForRightHand")));
}

// Called when the game starts or when spawned
void ALockedDoor::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void ALockedDoor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (bGetKey)
	{

		if (Door1->RelativeRotation.Yaw < 60.0f)
		{
			Door1->SetRelativeRotation(FMath::Lerp(Door1->RelativeRotation, AutoRot, 0.01f));
			Door2->SetRelativeRotation(FMath::Lerp(Door2->RelativeRotation, AutoRot, 0.01f));
		}
	}
}

void ALockedDoor::OpenDoor()
{
	bGetKey = true;
	Door1->SetCollisionProfileName("NoCollision");
	Door2->SetCollisionProfileName("NoCollision");

	Body->DestroyComponent();
	Button->DestroyComponent();
	Opener->DestroyComponent();
	Chain->DestroyComponent();
	BoxCollision->DestroyComponent();
	Key->Destroy();
}

void ALockedDoor::GetLockKey()
{
	FActorSpawnParameters SpawnActorOption; //액터를 스폰할때 쓰일 구조체 변수
	SpawnActorOption.Owner = this; //스폰할 액터의 주인을 현재 클래스로 정한다.
	SpawnActorOption.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;//액터를 스폰할때 충돌과 관계없이 스폰시킨다.
	FAttachmentTransformRules AttachRules(EAttachmentRule::SnapToTarget, EAttachmentRule::SnapToTarget, EAttachmentRule::KeepWorld, false);

	Key = GetWorld()->SpawnActor<ALockKey>(Key->StaticClass(), KeySocket->GetComponentLocation(), KeySocket->GetComponentRotation(), SpawnActorOption);

	if (Key)
	{
		Key->AttachToComponent(KeySocket, AttachRules);
	}

	BoxCollision->bGenerateOverlapEvents = false;
	Body->bGenerateOverlapEvents = false;
	Button->bGenerateOverlapEvents = false;
	Chain->bGenerateOverlapEvents = false;

	GetWorld()->GetTimerManager().SetTimer(OpenHandle, this, &ALockedDoor::OpenDoor, 2.0f, false);
}

void ALockedDoor::OpenDoors()
{
	Scene2->DestroyComponent();
	Key->Destroy();
	Destroy();
}
