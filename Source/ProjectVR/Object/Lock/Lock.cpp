// Fill out your copyright notice in the Description page of Project Settings.

#include "Lock.h"
#include "Components/StaticMeshComponent.h"
#include "UObject/ConstructorHelpers.h"
#include "Engine/StaticMesh.h"

// Sets default values
ALock::ALock()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Scene = CreateDefaultSubobject<USceneComponent>(TEXT("Scene"));
	Scene->SetupAttachment(RootComponent);

	Body = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Body"));
	Body->SetupAttachment(Scene);
	Button = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Button"));
	Button->SetupAttachment(Scene);
	Opener = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Opener"));
	Opener->SetupAttachment(Scene);
	Chain = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Chain"));
	Chain->SetupAttachment(Scene);

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

	Scene->SetRelativeScale3D(FVector(0.3f, 0.3f, 0.3f));

	Chain->SetRelativeScale3D(FVector(7.5f, 7.5f, 7.5f));
	Chain->SetRelativeLocation(FVector(-2.0f, -218.0f, 127.0f));
	Chain->SetWorldRotation(FRotator(0.0f, 0.0f, -57.0f));

	Body->SetEnableGravity(false);
	Body->bGenerateOverlapEvents = true;

	Body->SetCollisionProfileName("OverlapAll");
	Button->SetCollisionProfileName("OverlapAll");
	Chain->SetCollisionProfileName("OverlapAll");
	Tags.Add("Lock");

}

// Called when the game starts or when spawned
void ALock::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ALock::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ALock::OnOverlap(UPrimitiveComponent * OverlappedComp, AActor * OtherActor, UPrimitiveComponent * OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult)
{
	if (OtherActor->ActorHasTag("Key"))
	{
		OpenEvent.ExecuteIfBound();
	}
}

