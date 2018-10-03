// Fill out your copyright notice in the Description page of Project Settings.

#include "Lever.h"
#include "Components/StaticMeshComponent.h"
#include "UObject/ConstructorHelpers.h"					// 콘텐츠 브라우저로부터 포션의 모양을 가져올 때 사용
#include	"MyCharacter/MotionControllerCharacter.h"	// 오른손과 상호작용하기 위해서 캐릭터에 RightGrab에 대한 bool변수를 두었읆
#include "Kismet/KismetMathLibrary.h"
#include "Kismet/GameplayStatics.h"							// 캐릭터 찾기
#include "HandMotionController/RightHandMotionController.h"
#include "Engine/World.h"
#include "Engine/StaticMesh.h"

// Sets default values
ALever::ALever()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// 모양 생성
	LeverObject = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("LeverObj"));
	LeverObject->SetupAttachment(RootComponent);

	LeverScene = CreateDefaultSubobject<USceneComponent>(TEXT("LeverScene"));
	LeverScene->SetupAttachment(LeverObject);

	Lever = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Lever"));
	Lever->SetupAttachment(LeverScene);

	/////////////////////////////////////////////////////////////////////////////////////

	static ConstructorHelpers::FObjectFinder<UStaticMesh>PotionShape(TEXT("StaticMesh'/Game/Assets/CharacterEquipment/StarterContent/Shapes/Shape_Cube.Shape_Cube'"));		// 레퍼런스 경로로 방패 매쉬를 찾음
	if (PotionShape.Succeeded())		// 검 메쉬를 찾았을 경우 실행
	{
		Lever->SetStaticMesh(PotionShape.Object);			// 스태틱 메쉬에 검 모양 설정
		LeverObject->SetStaticMesh(PotionShape.Object);			// 스태틱 메쉬에 검 모양 설정
	}

	LeverObject->SetRelativeScale3D(FVector(0.4, 0.2, 0.4));
	LeverObject->SetRelativeLocation(FVector(0.0, 0.0, 0.0));
	//LeverObject->SetSimulatePhysics(true);
	LeverObject->SetCollisionProfileName("OverlapAll");

	LeverScene->SetRelativeLocation(FVector(0.0, 0.0, 4.0));
	LeverScene->SetRelativeScale3D(FVector(0.37, 0.38, 2.3));

	Lever->SetRelativeScale3D(FVector(1.0, 1.0, 1.6));
	Lever->SetRelativeLocation(FVector(0.0, 0.0, 82.0));
	Lever->SetCollisionProfileName("OverlapAll");
	
	Tags.Add(FName("Door"));
}

// Called when the game starts or when spawned
void ALever::BeginPlay()
{
	Super::BeginPlay();

	Cur = FVector::ZeroVector;
	Pre = FVector::ZeroVector;

	Lever->OnComponentBeginOverlap.AddDynamic(this, &ALever::OnLeverOverlap);		// 오버랩 이벤트를 발생시킬 수 있도록 설정
	Lever->OnComponentEndOverlap.AddDynamic(this, &ALever::OnLeverEndOverlap);		// 오버랩 이벤트를 발생시킬 수 있도록 설정
}

// Called every frame
void ALever::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (TouchActor)
	{
		if (TouchActor->ActorHasTag("RightHand"))
		{
			AMotionControllerCharacter* Character = Cast<AMotionControllerCharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));
			if (Character)
			{
				ARightHandMotionController* RightHand = Cast<ARightHandMotionController>(TouchActor);
				if (RightHand)
				{
					if (RightHand->bisRightGrab)		// 참이면 상호작용 실행
					{
						FVector Cal = UKismetMathLibrary::InverseTransformLocation
						(GetActorTransform(), RightHand->GetActorLocation());
						Cal = Cal - LeverScene->GetComponentLocation();
						float Degree = UKismetMathLibrary::RadiansToDegrees(UKismetMathLibrary::Atan2(Cal.X, Cal.Z));
						LeverScene->SetRelativeRotation(FRotator(Degree, 0.0f, 0.0f));


						UE_LOG(LogTemp, Log, TEXT("%f / %f / %f //// %f / %f / %f"), RightHand->GetActorLocation().X, RightHand->GetActorLocation().Y, RightHand->GetActorLocation().Z, Cal.X, Cal.Y, Cal.Z);
					}
				}
			}
		}
	}
}

void ALever::OnLeverOverlap(UPrimitiveComponent * OverlappedComp, AActor * OtherActor, UPrimitiveComponent * OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult)
{
	// 오른손과 상호작용
	TouchActor = OtherActor;

	if (OtherActor->ActorHasTag("RightHand"))
	{
		AMotionControllerCharacter* Character = Cast<AMotionControllerCharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));
		if (Character)
		{
			ARightHandMotionController* RightHand = Cast<ARightHandMotionController>(OtherActor);

			if (RightHand)
			{
				if (RightHand->bisRightGrab)
				{
					TouchActor = Character->RightHand;
				}
			}
		}
	}
}

void ALever::OnLeverEndOverlap(UPrimitiveComponent * OverlappedComponent, AActor * OtherActor, UPrimitiveComponent * OtherComp, int32 OtherBodyIndex)
{
	//// 오른손과 상호작용
	//if (OtherActor->ActorHasTag("RightHand"))
	//{
	//	TouchActor = nullptr;
	//	if (Interactor)
	//		Interactor = nullptr;
	//	UE_LOG(LogTemp, Log, TEXT("interactor with righthand out"));
	//}
}

