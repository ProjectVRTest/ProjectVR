// Fill out your copyright notice in the Description page of Project Settings.

#include "Lever.h"
#include "Components/StaticMeshComponent.h"
#include "UObject/ConstructorHelpers.h"					// ������ �������κ��� ������ ����� ������ �� ���
#include	"MyCharacter/MotionControllerCharacter.h"	// �����հ� ��ȣ�ۿ��ϱ� ���ؼ� ĳ���Ϳ� RightGrab�� ���� bool������ �ξ���
#include "Kismet/KismetMathLibrary.h"
#include "Kismet/GameplayStatics.h"							// ĳ���� ã��
#include "HandMotionController/RightHandMotionController.h"
#include "Engine/World.h"
// Sets default values
ALever::ALever()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// ��� ����
	LeverObject = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("LeverObj"));
	LeverObject->SetupAttachment(RootComponent);

	LeverScene = CreateDefaultSubobject<USceneComponent>(TEXT("LeverScene"));
	LeverScene->SetupAttachment(LeverObject);

	Lever = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Lever"));
	Lever->SetupAttachment(LeverScene);

	/////////////////////////////////////////////////////////////////////////////////////

	static ConstructorHelpers::FObjectFinder<UStaticMesh>PotionShape(TEXT("StaticMesh'/Game/Assets/Geometry/Meshes/1M_Cube.1M_Cube'"));		// ���۷��� ��η� ���� �Ž��� ã��
	if (PotionShape.Succeeded())		// �� �޽��� ã���� ��� ����
	{
		Lever->SetStaticMesh(PotionShape.Object);			// ����ƽ �޽��� �� ��� ����
		LeverObject->SetStaticMesh(PotionShape.Object);			// ����ƽ �޽��� �� ��� ����
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

	Lever->OnComponentBeginOverlap.AddDynamic(this, &ALever::OnLeverOverlap);		// ������ �̺�Ʈ�� �߻���ų �� �ֵ��� ����
	Lever->OnComponentEndOverlap.AddDynamic(this, &ALever::OnLeverEndOverlap);		// ������ �̺�Ʈ�� �߻���ų �� �ֵ��� ����
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
				ARightHandMotionController* RightHand = Cast<ARightHandMotionController>(Character->RightHand);
				if (RightHand)
				{
					if (RightHand->bisRightGrab)		// ���̸� ��ȣ�ۿ� ����
					{
						FVector Cal = UKismetMathLibrary::Subtract_VectorVector(UKismetMathLibrary::InverseTransformLocation
								(this->GetActorTransform(), TouchActor->GetActorLocation()), LeverScene->GetComponentLocation());
						
						LeverScene->SetRelativeRotation(FRotator(UKismetMathLibrary::Clamp(UKismetMathLibrary::Atan2(Cal.Z, Cal.X), 0.0f, 90.0f), 0.0f, 0.0f));


						UE_LOG(LogTemp, Log, TEXT("%f"), UKismetMathLibrary::Atan2(Cal.Z, Cal.X));
					}
				}
			}
		}














		//AMotionControllerCharacter* Character = Cast<AMotionControllerCharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));
		//if (Character)	// Interactor������ �������� �����ִµ� �������� �׷���ư�� ���� �ۿ��� ���� ���ϰ� �Ѵ�.
		//{
		//	ARightHandMotionController* RightHand = Cast<ARightHandMotionController>(Interactor);
		//	if (RightHand)
		//	{
		//		if(RightHand->bisRightGrab == false)
		//			Interactor = nullptr;
		//		else	// Grab����. ��ȣ�ۿ�
		//		{
		//			FVector Cal = UKismetMathLibrary::Subtract_VectorVector(UKismetMathLibrary::InverseTransformLocation
		//			(this->GetActorTransform(), Interactor->GetActorLocation()), LeverScene->GetComponentLocation());

		//			LeverScene->SetRelativeRotation(FRotator(0.0f, UKismetMathLibrary::Clamp(UKismetMathLibrary::Atan2(Cal.Z, Cal.X), 0.0f, 90.0f), 0.0f));
		//		}
		//	}
		//}
	}
}

void ALever::OnLeverOverlap(UPrimitiveComponent * OverlappedComp, AActor * OtherActor, UPrimitiveComponent * OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult)
{
	// �����հ� ��ȣ�ۿ�
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
	// �����հ� ��ȣ�ۿ�
	if (OtherActor->ActorHasTag("RightHand"))
	{
		TouchActor = nullptr;
		if (Interactor)
			Interactor = nullptr;
		UE_LOG(LogTemp, Log, TEXT("interactor with righthand out"));
	}
}

