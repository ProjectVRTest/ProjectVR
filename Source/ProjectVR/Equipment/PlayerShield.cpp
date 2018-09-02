// Fill out your copyright notice in the Description page of Project Settings.

#include "PlayerShield.h"
#include "Components/WidgetComponent.h"			// 위젯 컴포넌트
#include "Components/StaticMeshComponent.h"		// 스태틱 메쉬 컴포넌트
#include "UObject/ConstructorHelpers.h"				// ConstructorHelpers 사용

#include "Runtime/Engine/Classes/Materials/MaterialParameterCollectionInstance.h"
#include "Runtime/Engine/Classes/Materials/MaterialParameterCollection.h"
#include "Engine/StaticMesh.h"
#include "Monster/MiniBoss/Weapon/MiniBossWeapon.h"
#include "Monster/MiniBoss/MiniBoss.h"
#include "Kismet/GameplayStatics.h"				// 오너 설정
#include "MyCharacter/MotionControllerCharacter.h"	// 오너 설정
// Sets default values
APlayerShield::APlayerShield()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	
	/* 스태틱 매쉬 컴포넌트 생성 */
	ShieldMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ShieldMesh"));
	ShieldMesh->SetCollisionProfileName(TEXT("OverlapAll"));			// 캐릭터와의 충돌을 피하기위해서 Pawn(캐릭터)만 Overlap되도록 설정
	SetRootComponent(ShieldMesh);

	ShieldMesh->SetRelativeScale3D(FVector(-1.0f, 1.0f, 1.0f));

	static ConstructorHelpers::FObjectFinder<UStaticMesh>SM_Shield(TEXT("StaticMesh'/Game/Assets/Equipment/Shield/Mesh/SM_Shield.SM_Shield'"));		// 레퍼런스 경로로 방패 매쉬를 찾음
	if (SM_Shield.Succeeded())		// 방패 메쉬를 찾았을 경우 실행
	{
		ShieldMesh->SetStaticMesh(SM_Shield.Object);		// 스태틱 메쉬에 방패 모양 설정
	}

	
	IsActivation = false;
	IsMiniBossWeaponOverlap = false;
	Tags.Add(FName(TEXT("PlayerShield")));		// 생성한 방패를 'PlayerShield'란 이름으로 태그를 줌
	Tags.Add(FName(TEXT("DisregardForLeftHand")));
	Tags.Add(FName(TEXT("DisregardForRightHand")));
}

// Called when the game starts or when spawned
void APlayerShield::BeginPlay()
{
	Super::BeginPlay();
	
	// 오너 설정
	ShieldOwner = Cast<AMotionControllerCharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));

	if (ShieldMesh)
	{
		ShieldMesh->OnComponentBeginOverlap.AddDynamic(this, &APlayerShield::OnShieldOverlapStart);
		ShieldMesh->OnComponentEndOverlap.AddDynamic(this, &APlayerShield::OnShieldOverlapEnd);
	}
}

// Called every frame
void APlayerShield::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (IsActivation && IsMiniBossWeaponOverlap)
	{
		if(MiniBoss)
		{
			GLog->Log(FString::Printf(TEXT("방패 패링")));
			MiniBoss->CurrentAnimState = EMiniBossAnimState::ParryingReady;
		}
	}
	//GLog->Log(FString::Printf(TEXT("%d"), IsMiniBossWeaponOverlap));
	//UE_LOG(LogTemp, Log, TEXT(" ** %f"), ShieldMesh->GetPhysicsLinearVelocity().Size());
}

void APlayerShield::ConvertOfOpacity(float opacity)		// Opacity값 세팅(캐릭터에서 호출)
{
	if (ShieldMesh)
	{
		ShieldMesh->SetScalarParameterValueOnMaterials(FName(TEXT("ShieldOpacity")), opacity);		
	}

	if (IsActivation)
	{
		IsActivation = false;
	}
	else
	{
		IsActivation = true;
	}
}

void APlayerShield::OnShieldOverlapStart(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult)
{
	if (OtherActor->ActorHasTag(TEXT("MiniBossWeapon")))
	{
		IsMiniBossWeaponOverlap = true;

		if (IsActivation && IsMiniBossWeaponOverlap)
		{
			AMiniBossWeapon* MiniBossWeapon = Cast<AMiniBossWeapon>(OtherActor);

			if (MiniBossWeapon)
			{
				if (MiniBossWeapon->IsWeaponAttack)
				{
					MiniBoss = Cast<AMiniBoss>(MiniBossWeapon->GetAttachParentActor());

					/*if (MiniBoss)
					{
						GLog->Log(FString::Printf(TEXT("방패 패링")));
						MiniBoss->CurrentAnimState = EMiniBossAnimState::ParryingReady;
					}
					else
					{
						GLog->Log(FString::Printf(TEXT("Null")));
					}*/
				}
			}
		}

		if (ShieldMesh->GetPhysicsLinearVelocity().Size() >= 50.0f)
		{
			GLog->Log(FString::Printf(TEXT("속력 50이상 판정")));
		}		
	}
}

void APlayerShield::OnShieldOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (OtherActor->ActorHasTag(TEXT("MiniBossWeapon")))
	{
		GLog->Log(FString::Printf(TEXT("칼에서 벗어남")));
		IsMiniBossWeaponOverlap = false;
		MiniBoss = nullptr;
	}	
}