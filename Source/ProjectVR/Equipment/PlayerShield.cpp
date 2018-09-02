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
#include "Particles/ParticleSystem.h"
#include "kismet/GameplayStatics.h"
#include "Components/BoxComponent.h"

#include "Haptics/HapticFeedbackEffect_Base.h"
#include "MyCharacter/MotionControllerPC.h"
#include "HandMotionController/LeftHandMotionController.h"
#include "MyCharacter/MotionControllerCharacter.h"	// 오너 설정
// Sets default values
APlayerShield::APlayerShield()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	
	/* 스태틱 매쉬 컴포넌트 생성 */
	ShieldMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ShieldMesh"));
	ShieldMesh->SetCollisionProfileName(TEXT("NoCollision"));			// 캐릭터와의 충돌을 피하기위해서 Pawn(캐릭터)만 Overlap되도록 설정
	SetRootComponent(ShieldMesh);

	ShieldMesh->SetRelativeScale3D(FVector(-1.0f, 1.0f, 1.0f));

	static ConstructorHelpers::FObjectFinder<UStaticMesh>SM_Shield(TEXT("StaticMesh'/Game/Assets/Equipment/Shield/Mesh/SM_Shield.SM_Shield'"));		// 레퍼런스 경로로 방패 매쉬를 찾음
	if (SM_Shield.Succeeded())		// 방패 메쉬를 찾았을 경우 실행
	{
		ShieldMesh->SetStaticMesh(SM_Shield.Object);		// 스태틱 메쉬에 방패 모양 설정
	}

	static ConstructorHelpers::FObjectFinder<UParticleSystem>PT_ParryingEffect(TEXT("ParticleSystem'/Game/Assets/StarterContent/Particles/P_Explosion.P_Explosion'"));
	if (PT_ParryingEffect.Succeeded())
	{
		ParryingEffect = PT_ParryingEffect.Object;
	}

	static ConstructorHelpers::FObjectFinder<UHapticFeedbackEffect_Base> HapticEffect(TEXT("HapticFeedbackEffect_Curve'/Game/Assets/MyCharacter/Hand/HandHaptics.HandHaptics'"));

	if (HapticEffect.Succeeded())
	{
		ShieldHapticEffect = HapticEffect.Object;
	}	

	ShieldCollision = CreateDefaultSubobject<UBoxComponent>(TEXT("ShieldCollsion"));
	ShieldCollision->SetupAttachment(ShieldMesh);
	ShieldCollision->SetRelativeLocation(FVector(-2.2f, 10.0f, 1.4f));
	ShieldCollision->SetRelativeRotation(FRotator(0, -20.0f, 0));
	ShieldCollision->SetRelativeScale3D(FVector(0.75f, 1.5f, 0.24f));
	ShieldCollision->SetCollisionProfileName(TEXT("OverlapAll"));
	ShieldCollision->bHiddenInGame = false;
	IsActivation = false;
	
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
		ShieldCollision->OnComponentBeginOverlap.AddDynamic(this, &APlayerShield::OnShieldOverlapStart);
		ShieldCollision->OnComponentEndOverlap.AddDynamic(this, &APlayerShield::OnShieldOverlapEnd);
	}
}

// Called every frame
void APlayerShield::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (ShieldMesh->GetPhysicsLinearVelocity().Size() > 300.0f)
	{
		GLog->Log(FString::Printf(TEXT("방패 속도 : %0.1f"), ShieldMesh->GetPhysicsLinearVelocity().Size()));
	}

	if (ShieldOwner)
	{
		ShieldPhysicsVelocityValue = ShieldCollision->GetPhysicsLinearVelocity().Size() - ShieldOwner->GetVelocity().Size();
	}
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
		AMiniBossWeapon* MiniBossWeapon = Cast<AMiniBossWeapon>(OtherActor);

		if (MiniBossWeapon)
		{
			if (MiniBossWeapon->IsParryingAttack)
			{
				MiniBoss = Cast<AMiniBoss>(MiniBossWeapon->GetAttachParentActor());
				if (MiniBoss)
				{
					if (IsActivation && ShieldPhysicsVelocityValue > 300.0f) //그립버튼을 누르고 방패의 선속도가 300이상일때 판정
					{
						RumbleLeftController(5.0f);
						MiniBossWeapon->IsParryingAttack = false;
						UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), ParryingEffect, MiniBoss->GetActorLocation());
						//SweepResult.
						GLog->Log(FString::Printf(TEXT("방패 패링")));
						MiniBoss->CurrentAnimState = EMiniBossAnimState::ParryingReady;
					}
				}
				else
				{
					GLog->Log(FString::Printf(TEXT("Null")));
				}
			}
		}
	}
}

void APlayerShield::OnShieldOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (OtherActor->ActorHasTag(TEXT("MiniBossWeapon")))
	{

	}	
}

void APlayerShield::RumbleLeftController(float Intensity)
{
	AMotionControllerPC* PC = Cast<AMotionControllerPC>(UGameplayStatics::GetPlayerController(GetWorld(), 0));

	if (PC)
	{
		ALeftHandMotionController* LeftHand = Cast<ALeftHandMotionController>(GetAttachParentActor());

		if (LeftHand)
		{
			PC->PlayHapticEffect(ShieldHapticEffect, LeftHand->Hand, Intensity);
		}		
	}
}
