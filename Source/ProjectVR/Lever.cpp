// Fill out your copyright notice in the Description page of Project Settings.

#include "Lever.h"
#include "Components/StaticMeshComponent.h"
#include "UObject/ConstructorHelpers.h"					// 콘텐츠 브라우저로부터 포션의 모양을 가져올 때 사용
#include	"MyCharacter/MotionControllerCharacter.h"	// 오른손과 상호작용하기 위해서 캐릭터에 RightGrab에 대한 bool변수를 두었읆
#include "Kismet/KismetMathLibrary.h"
#include "Kismet/GameplayStatics.h"							// 캐릭터 찾기
#include "HandMotionController/RightHandMotionController.h"
#include "HandMotionController/LeftHandMotionController.h"
#include "Engine/World.h"
#include "Engine/StaticMesh.h"

#include"Components/BoxComponent.h"
#include"Components/StaticMeshComponent.h"
// Sets default values
ALever::ALever()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Scene = CreateDefaultSubobject<USceneComponent>(TEXT("Scene"));
	Scene->SetupAttachment(RootComponent);

	LeverScene = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("LeverScene"));
	LeverScene->SetupAttachment(Scene);

	Lever = CreateDefaultSubobject<UBoxComponent>(TEXT("Lever"));
	Lever->SetupAttachment(LeverScene);

	Collision = CreateDefaultSubobject<UBoxComponent>(TEXT("Collision"));
	Collision->SetupAttachment(Lever);


	static ConstructorHelpers::FObjectFinder<UStaticMesh>SM_Door(TEXT("StaticMesh'/Game/Assets/MapBuild/RoughMap/Cathedral_door/backup/Main_door.Main_door'"));		// 레퍼런스 경로로 방패 매쉬를 찾음
	if (SM_Door.Succeeded())		// 검 메쉬를 찾았을 경우 실행
	{
		LeverScene->SetStaticMesh(SM_Door.Object);			// 스태틱 메쉬에 검 모양 설정
	}

	Scene->SetRelativeScale3D(FVector(0.117701f, 0.117701f, 0.117701f));

	LeverScene->SetRelativeScale3D(FVector(4.0f, 4.0f, 4.0f));

	Lever->SetRelativeLocation(FVector(-99.801483f, 2.628052f, 191.0f));
	Lever->SetRelativeScale3D(FVector(0.318f, 0.787583f, 1.380393f));

	Collision->SetRelativeLocation(FVector(148.561737f, 0.259847f, 15.835767f));
	Collision->SetRelativeScale3D(FVector(4.620687f, 0.188942f, 3.429634f));

	AutoRot = FRotator(LeverScene->RelativeRotation.Pitch, LeverScene->RelativeRotation.Yaw + 85.0f, LeverScene->RelativeRotation.Roll);
	DefaultYaw = LeverScene->GetComponentRotation().Yaw;

	Collision->bGenerateOverlapEvents = false;

	LeverScene->SetCollisionProfileName("NoCollision");
	Lever->SetCollisionProfileName("OverlapAll");
	Collision->SetCollisionProfileName("BlockAll");

	Tags.Add(FName("Door"));
}

// Called when the game starts or when spawned
void ALever::BeginPlay()
{
	Super::BeginPlay();

	Lever->OnComponentBeginOverlap.AddDynamic(this, &ALever::OnLeverOverlap);		// 오버랩 이벤트를 발생시킬 수 있도록 설정
	Lever->OnComponentEndOverlap.AddDynamic(this, &ALever::OnLeverEndOverlap);		// 오버랩 이벤트를 발생시킬 수 있도록 설정
}

// Called every frame
void ALever::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (TouchActor)
	{
		ARightHandMotionController* RightHand = Cast<ARightHandMotionController>(TouchActor);
		ALeftHandMotionController* LeftHand = Cast<ALeftHandMotionController>(TouchActor);
		if (RightHand)
		{
			if (RightHand->bisRightGrab)		// 참이면 상호작용 실행
			{
				FVector Cal = UKismetMathLibrary::InverseTransformLocation
				(GetActorTransform(), RightHand->GetActorLocation());

				float degree = UKismetMathLibrary::RadiansToDegrees(UKismetMathLibrary::Atan2(-Cal.Y, -Cal.X));

				if (degree > 0.0f)
					LeverScene->SetRelativeRotation(FRotator(0.0f, degree, 0.0f));
			}
		}
		else if (LeftHand)
		{
			if (LeftHand->bisLeftGrab)		// 참이면 상호작용 실행
			{
				FVector Cal = UKismetMathLibrary::InverseTransformLocation
				(GetActorTransform(), LeftHand->GetActorLocation());

				float degree = UKismetMathLibrary::RadiansToDegrees(UKismetMathLibrary::Atan2(-Cal.Y, -Cal.X));

				if (degree > 0.0f)
					LeverScene->SetRelativeRotation(FRotator(0.0f, degree, 0.0f));
			}
		}
	}

	if (LeverScene->RelativeRotation.Yaw > 10.0f)
	{
		LeverScene->SetRelativeRotation(FMath::Lerp(LeverScene->RelativeRotation, AutoRot, 0.01f));
	}

}

void ALever::OnLeverOverlap(UPrimitiveComponent * OverlappedComp, AActor * OtherActor, UPrimitiveComponent * OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult)
{
	if (OtherActor->ActorHasTag("RightHand"))
	{
		AMotionControllerCharacter* Character = Cast<AMotionControllerCharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));
		if (Character)
		{
			ARightHandMotionController* RightHand = Cast<ARightHandMotionController>(OtherActor);

			if (RightHand)
				TouchActor = Character->RightHand;
		}
	}

	if (OtherActor->ActorHasTag("LeftHand"))
	{
		AMotionControllerCharacter* Character = Cast<AMotionControllerCharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));
		if (Character)
		{
			ALeftHandMotionController* RightHand = Cast<ALeftHandMotionController>(OtherActor);

			if (RightHand)
				TouchActor = Character->RightHand;
		}
	}
}

void ALever::OnLeverEndOverlap(UPrimitiveComponent * OverlappedComponent, AActor * OtherActor, UPrimitiveComponent * OtherComp, int32 OtherBodyIndex)
{

}

