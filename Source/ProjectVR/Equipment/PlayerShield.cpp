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
#include "MyCharacter/MotionControllerCharacter.h"	// Setting Owner
#include "MyCharacter/Widget/HPStaminaBar.h"			// Character State Bar
// Sets default values
APlayerShield::APlayerShield()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	
	/* 스태틱 매쉬 컴포넌트 생성 */
	ShieldMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ShieldMesh"));
	SetRootComponent(ShieldMesh);
	static ConstructorHelpers::FObjectFinder<UStaticMesh>SM_Shield(TEXT("StaticMesh'/Game/Assets/Equipment/Shield/Mesh/SM_Shield.SM_Shield'"));		// 레퍼런스 경로로 방패 매쉬를 찾음
	if (SM_Shield.Succeeded())		// 방패 메쉬를 찾았을 경우 실행
	{
		ShieldMesh->SetStaticMesh(SM_Shield.Object);		// 스태틱 메쉬에 방패 모양 설정
	}

	// 방패 콜리전 
	ShieldCollision = CreateDefaultSubobject<UBoxComponent>(TEXT("ShieldCollsion"));
	ShieldCollision->SetupAttachment(ShieldMesh);

	// 상태 바
	StateBarScene = CreateDefaultSubobject<USceneComponent>(TEXT("StateScene"));
	StateBarScene->SetupAttachment(ShieldMesh);

	static ConstructorHelpers::FObjectFinder<UParticleSystem>PT_ParryingEffect(TEXT("ParticleSystem'/Game/Assets/StarterContent/Particles/P_Explosion.P_Explosion'"));
	if (PT_ParryingEffect.Succeeded())
	{
		ParryingEffect = PT_ParryingEffect.Object;
	}

	StateBar = nullptr;
	ShieldMesh->SetRelativeScale3D(FVector(-1.0f, 1.0f, 1.0f));
	ShieldMesh->SetCollisionProfileName(TEXT("NoCollision"));			// 캐릭터와의 충돌을 피하기위해서 Pawn(캐릭터)만 Overlap되도록 설정

	// 위치, 각도 및 크기 설정
	ShieldCollision->SetRelativeLocation(FVector(-2.2f, 10.0f, 1.4f));
	ShieldCollision->SetRelativeRotation(FRotator(0, -20.0f, 0));
	ShieldCollision->SetRelativeScale3D(FVector(0.75f, 1.5f, 0.24f));
	ShieldCollision->SetCollisionProfileName(TEXT("OverlapAll"));
	ShieldCollision->bHiddenInGame = false;

	// 스테이트바 씬의 위기값과 회전값 설정
	StateBarScene->SetRelativeLocation(FVector(-23.0f, -10.0f, 0.0f));
	StateBarScene->SetRelativeRotation(FRotator(0.0f, -19.0f, -90.0f));
	IsActivation = false;
	
	// 태그
	Tags.Add(FName(TEXT("PlayerShield")));
	Tags.Add(FName(TEXT("DisregardForLeftHand")));
	Tags.Add(FName(TEXT("DisregardForRightHand")));
}

// Called when the game starts or when spawned
void APlayerShield::BeginPlay()
{
	Super::BeginPlay();

	FActorSpawnParameters SpawnActorOption;
	SpawnActorOption.Owner = this; 
	SpawnActorOption.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
	FAttachmentTransformRules AttachRules(EAttachmentRule::SnapToTarget, EAttachmentRule::SnapToTarget, EAttachmentRule::KeepWorld, false);

	////방패를 쉴드 씬 컴포넌트에 스폰시킨다.
	StateBar = GetWorld()->SpawnActor<AHPStaminaBar>(StateBar->StaticClass(), StateBarScene->GetComponentLocation(), StateBarScene->GetComponentRotation(), SpawnActorOption);
	////방패를 AttachRules를 토대로 쉴드 씬 컴포넌트에 붙인다.
	StateBar->AttachToComponent(StateBarScene, AttachRules);
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
			PC->RumbleHandController(LeftHand->Hand,Intensity);
		}		
	}
}
