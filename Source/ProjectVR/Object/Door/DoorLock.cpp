// Fill out your copyright notice in the Description page of Project Settings.

#include "DoorLock.h"
#include "Components/StaticMeshComponent.h"
#include "UObject/ConstructorHelpers.h"
#include "Engine/StaticMesh.h"
#include "Kismet/GameplayStatics.h"							// 캐릭터 찾기
#include "MyCharacter/MotionControllerCharacter.h"
#include "Components/BoxComponent.h"
#include "Object/Door/LockKey.h"		// 문열 때 키 생성
#include "Engine/World.h"
#include "Public/TimerManager.h" 

// Sets default values
ADoorLock::ADoorLock()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Scene = CreateDefaultSubobject<USceneComponent>(TEXT("Scene"));
	Scene->SetupAttachment(RootComponent);

	LockScene = CreateDefaultSubobject<USceneComponent>(TEXT("LockScene"));
	LockScene->SetupAttachment(Scene);

	CollisionScene = CreateDefaultSubobject<USceneComponent>(TEXT("CollisionScene"));
	CollisionScene->SetupAttachment(Scene);

	KeySocket = CreateDefaultSubobject<USceneComponent>(TEXT("KeySocket"));
	KeySocket->SetupAttachment(Scene);

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

	BoxCollision->ComponentTags.Add("LockArea");
	Body->ComponentTags.Add("BodyLock");

	Tags.Add("Lock");
	Tags.Add(FName(TEXT("DisregardForLeftHand")));
	Tags.Add(FName(TEXT("DisregardForRightHand")));
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

void ADoorLock::GetLockKey()
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

	GetWorld()->GetTimerManager().SetTimer(OpenHandle, this, &ADoorLock::OpenDoor, 2.0f, false);
}

void ADoorLock::OpenDoor()
{
	//OpenEvent.ExecuteIfBound();
	Key->Destroy();
	Destroy();
}

