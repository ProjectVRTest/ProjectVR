// Fill out your copyright notice in the Description page of Project Settings.

#include "PlayerSword.h"
#include "Components/StaticMeshComponent.h"			// 스태틱메쉬 컴포넌트
#include "UObject/ConstructorHelpers.h"					// ConstructorHelpers 사용
#include "Components/CapsuleComponent.h"				// 캡슐컴포넌트
#include "Particles/ParticleSystem.h" //파티클 이펙트에 사용할 헤더
#include "Kismet/GameplayStatics.h"							// 데미지전달시 사용 / 오너 설정

#include "MyCharacter/MotionControllerCharacter.h"

#include "Engine/StaticMesh.h"

#include "Haptics/HapticFeedbackEffect_Base.h"
#include "MyCharacter/MotionControllerPC.h"
#include "HandMotionController/RightHandMotionController.h"
#include "Monster/Dog/Dog.h"
#include "Monster/Boss/Orb/Ultimate/Wave/BossRedOrbWave.h"
#include "Monster/Boss/Orb/DefaultOrb/BossOrb.h"
#include "Components/AudioComponent.h"
#include "Sound/SoundCue.h"

// Sets default values
APlayerSword::APlayerSword()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	/* 스태틱 매쉬 컴포넌트 생성 */
	SwordMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("SwordMesh"));
	SetRootComponent(SwordMesh);
	SwordMesh->SetCollisionProfileName(TEXT("NoCollision"));		// 메쉬의 콜리전 상태값을 NoCollision으로 줌.

	static ConstructorHelpers::FObjectFinder<UStaticMesh>SM_Sword(TEXT("StaticMesh'/Game/Assets/CharacterEquipment/Equipment/Sword/Mesh/SM_Sword.SM_Sword'"));		// 레퍼런스 경로로 방패 매쉬를 찾음
	if (SM_Sword.Succeeded())		// 검 메쉬를 찾았을 경우 실행
	{
		SwordMesh->SetStaticMesh(SM_Sword.Object);			// 스태틱 메쉬에 검 모양 설정
	}

	/* 콜리전 컴포넌트 생성 */
	SwordCollision = CreateDefaultSubobject<UCapsuleComponent>(TEXT("SwordCollision"));
	SwordCollision->SetupAttachment(SwordMesh);		// 콜리전을 검 메쉬에 붙임
	SwordCollision->SetCollisionProfileName(TEXT("OverlapAll"));	

	// 콜리전 위치 및 방향, 크기 설정
	SwordCollision->SetRelativeLocation(FVector(0.0f, 90.0f, 0.0f));
	SwordCollision->SetRelativeRotation(FRotator(0.0f, 0.0f, 90.0f));
	SwordCollision->SetRelativeScale3D(FVector(0.75f, 0.75f, 1.6f));
	SwordCollision->ComponentTags.Add(FName(TEXT("PlayerSwordCollision")));
	
	SwordSoundComponent = CreateDefaultSubobject<UAudioComponent>(TEXT("SwordSoundComponent"));
	SwordSoundComponent->SetupAttachment(SwordMesh);
	SwordSoundComponent->SetRelativeLocation(FVector(0, 85.0f, 0));
	
	static ConstructorHelpers::FObjectFinder<UParticleSystem>PT_BloodEffect(TEXT("ParticleSystem'/Game/Assets/Effect/HitFeedback/Blood_cloud_large.Blood_cloud_large'"));
	if (PT_BloodEffect.Succeeded())
	{
		BloodEffect = PT_BloodEffect.Object;
	}
	
	static ConstructorHelpers::FObjectFinder<UParticleSystem>PT_OrbHitEffect(TEXT("ParticleSystem'/Game/Assets/Effect/HitFeedback/PT_OrbHitEffect.PT_OrbHitEffect'"));
	if (PT_OrbHitEffect.Succeeded())
	{
		OrbHitEffect = PT_OrbHitEffect.Object;
	}

	static ConstructorHelpers::FObjectFinder<USoundCue>PlayerSwordSoundAsset(TEXT("SoundCue'/Game/Assets/Sounds/NormalMob/PlayerSwordSound.PlayerSwordSound'"));
	if (PlayerSwordSoundAsset.Succeeded())
	{
		SwordSound = PlayerSwordSoundAsset.Object;
		SwordSoundComponent->SetSound(SwordSound);
	}

	Timer = 0.0f;		// 타이머 초기화

	// 태그
	Tags.Add(FName(TEXT("DisregardForLeftHand")));
	Tags.Add(FName(TEXT("DisregardForRightHand")));
}

// Called when the game starts or when spawned
void APlayerSword::BeginPlay()
{
	Super::BeginPlay();

	// 오너 설정
	SwordOwner = Cast<AMotionControllerCharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));

	// 공격
	if (SwordCollision)
	{
		SwordCollision->OnComponentBeginOverlap.AddDynamic(this, &APlayerSword::OnSwordOverlap);
	}
}

// Called every frame
void APlayerSword::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	Timer += DeltaTime;		// 타이머

	// 매 틱마다 캐릭터의 운동값을 뺌
	if (SwordOwner)
	{
		SwordCurrentPosistion = SwordCollision->GetComponentLocation() - GetActorLocation();
		SwordMoveDelta = SwordCurrentPosistion - SwordPreviousPosistion;
		SwordMoveVelocity = SwordMoveDelta / DeltaTime;
		SwordPreviousPosistion = SwordCurrentPosistion;

		if (SwordMoveVelocity.Size() >= 1000.0f)
		{
			if (!SwordSoundComponent->IsPlaying())
			{
				SwordSoundComponent->Play(0);
			}			
		}
	}
}

void APlayerSword::OnSwordOverlap(UPrimitiveComponent * OverlappedComp, AActor * OtherActor, UPrimitiveComponent * OtherComp,
	int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult)		// 오버랩이벤트시 실행할 것들
{
	if (OtherActor->ActorHasTag("Dog"))
	{
		ADog* Dog = Cast<ADog>(OtherActor);

		if (SwordOwner->RightHand->AttachDog == Dog)
			return;
	}

	if (OtherActor->ActorHasTag("Monster"))		// 오버랩된 액터가 'Monster'라는 태그를 가지고 있으면 실행
	{			
		if (Timer >= 0.5f)			// 타이머가 0.5 이상의 수를 가지고 있을 때 실행 (조건1)
		{
			if (SwordMoveVelocity.Size() >= 1000.0f) //그립버튼을 누르고 선속도의 크기가 200 이상일 때만 공격 판정이 일어남 (조건2)
			{
				// 최소 스테미너이상 있을 때 공격 가능 
				if (SwordOwner->CurrentStamina >= SwordOwner->AttackPoint)
				{
					SwordOwner->UseStamina(SwordOwner->AttackPoint);			// 스테미너 감소 및 대기시간 후 스테미너 회복시작
					Timer = 0.0f;		// 공격 판정이 일어났을 때 타이머 0으로

					if (SwordMoveVelocity.Size() <= 1000.0f)// 선속도의 크기가 500이하일 때 데미지 10 (조건4)
					{
						RumbleRightController(0.5f);
						Damage = 10.0f;
					}
					else // 선속도의 크기가 500초과일 때 데미지 15 (조건4)
					{
						RumbleRightController(1.0f);
						Damage = 15.0f;
					}
					// 이하 캐릭터 스테미너 감소
					// 이하 UI스테미너 감소
					UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), BloodEffect, OtherActor->GetActorLocation());
					UGameplayStatics::ApplyDamage(OtherActor, Damage, UGameplayStatics::GetPlayerController(GetWorld(), 0), this, nullptr);		// 오버랩된 액터에 데미지 전달
					// (오버랩발생된 액터, 데미지, 데미지를가한 주체, 실제 데미지를 가한주체, 데미지종류클래스)
				}
			}
		}
	}

	if (OtherComp->ComponentHasTag(TEXT("BossRedOrbWave")))
	{
		if (SwordMoveVelocity.Size() >= 500.0f) 
		{
			ABossRedOrbWave* RedOrbWave = Cast<ABossRedOrbWave>(OtherComp->GetOwner());

			if (RedOrbWave)
			{
				UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), RedOrbWave->OrbWaveExplosion, RedOrbWave->GetActorLocation());
				RedOrbWave->Destroy();
			}
		}		
	}
	else if (OtherComp->ComponentHasTag(TEXT("BossDefaultOrb")))
	{
		if (SwordMoveVelocity.Size() >= 500.0f)
		{
			ABossOrb* DefaultOrb = Cast<ABossOrb>(OtherComp->GetOwner());

			if (DefaultOrb)
			{
				Damage = 10.0f;
				UGameplayStatics::ApplyDamage(OtherActor, Damage, UGameplayStatics::GetPlayerController(GetWorld(), 0), this, nullptr);				
			}
		}
	}
}

void APlayerSword::RumbleRightController(float Intensity)
{
	AMotionControllerPC* PC = Cast<AMotionControllerPC>(UGameplayStatics::GetPlayerController(GetWorld(), 0));

	if (PC)
	{
		ARightHandMotionController* RightHand = Cast<ARightHandMotionController>(GetAttachParentActor());

		if (RightHand)
		{
			PC->RumbleHandController(RightHand->Hand, Intensity);
		}
	}
}
