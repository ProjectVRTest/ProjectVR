// Fill out your copyright notice in the Description page of Project Settings.

#include "PlayerShield.h"
#include "Components/WidgetComponent.h"			// 위젯 컴포넌트
#include "Components/StaticMeshComponent.h"		// 스태틱 메쉬 컴포넌트
#include "UObject/ConstructorHelpers.h"				// ConstructorHelpers 사용

#include "Runtime/Engine/Classes/Materials/MaterialParameterCollectionInstance.h"
#include "Runtime/Engine/Classes/Materials/MaterialParameterCollection.h"
#include "Engine/StaticMesh.h"

#include "Monster/MiniBoss/MiniBoss.h"
#include "Monster/Boss/Boss.h"

#include "Particles/ParticleSystem.h"
#include "kismet/GameplayStatics.h"
#include "Components/BoxComponent.h"

#include "Haptics/HapticFeedbackEffect_Base.h"
#include "MyCharacter/MotionControllerPC.h"
#include "HandMotionController/LeftHandMotionController.h"
#include "MyCharacter/MotionControllerCharacter.h"	// Setting Owner
#include "MyCharacter/Widget/HPStaminaBar_2.h"			// Character State Bar
#include "MyCharacter/Widget/HPStaminaBar.h"			// Character State Bar
#include "TimerManager.h"

#include "Monster/Boss/Weapon/BossWeapon.h"
#include "Monster/Boss/Orb/DefaultOrb/BossOrbWave.h"
#include "Monster/Boss/Orb/Ultimate/Wave/BossBlueOrbWave.h"
#include "Monster/Boss/AI/AddAttack/BossAddAttackBall.h"
#include "Monster/Normal/ArcherSword/Weapon/Sword/NMWeaponSword.h"
#include "Monster/Normal/ArcherSword/Weapon/Bow/NMWeaponArrow.h"
#include "Monster/MiniBoss/Weapon/MiniBossWeapon.h"
#include "Monster/MiniBoss/Weapon/SwordWave/MiniBossSwordWave.h"
#include "Components/AudioComponent.h"
#include "Sound/SoundCue.h"
#include "Component/Monster/MonsterParryingManager.h"

// Sets default values
APlayerShield::APlayerShield()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	/* 스태틱 매쉬 컴포넌트 생성 */
	ShieldMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ShieldMesh"));
	SetRootComponent(ShieldMesh);
	static ConstructorHelpers::FObjectFinder<UStaticMesh>SM_Shield(TEXT("StaticMesh'/Game/Assets/CharacterEquipment/Equipment/Shield/Mesh/SM_Shield.SM_Shield'"));		// 레퍼런스 경로로 방패 매쉬를 찾음
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

	static ConstructorHelpers::FObjectFinder<UParticleSystem>PT_ParryingEffect(TEXT("ParticleSystem'/Game/Assets/Effect/HitFeedback/PT_ShieldParrying.PT_ShieldParrying'"));
	if (PT_ParryingEffect.Succeeded())
	{
		ParryingEffect = PT_ParryingEffect.Object;
	}

	static ConstructorHelpers::FObjectFinder<UParticleSystem>PT_ShieldBlockEffect(TEXT("ParticleSystem'/Game/Assets/Effect/HitFeedback/PT_ShieldBlock.PT_ShieldBlock'"));
	if (PT_ShieldBlockEffect.Succeeded())
	{
		ShieldBlockEffect = PT_ShieldBlockEffect.Object;
	}

	static ConstructorHelpers::FObjectFinder<UMaterialInterface> M_ShieldVisibleMaterial(TEXT("MaterialInstanceConstant'/Game/Assets/CharacterEquipment/Equipment/Shield/Materials/M_ShieldVisible_Inst.M_ShieldVisible_Inst'"));
	if (M_ShieldVisibleMaterial.Succeeded())
	{
		ShieldVisibleMaterial = M_ShieldVisibleMaterial.Object;
	}

	static ConstructorHelpers::FObjectFinder<UMaterialInterface> M_ShieldInVisibleMaterial(TEXT("MaterialInstanceConstant'/Game/Assets/CharacterEquipment/Equipment/Shield/Materials/M_ShieldInVisibleInst.M_ShieldInVisibleInst'"));
	if (M_ShieldInVisibleMaterial.Succeeded())
	{
		ShieldInVisibleMaterial = M_ShieldInVisibleMaterial.Object;
	}

	StateBar = nullptr;
	ShieldMesh->SetRelativeScale3D(FVector(1.0f, 1.0f, 1.0f));
	ShieldMesh->SetCollisionProfileName(TEXT("NoCollision"));

	// 위치, 각도 및 크기 설정
	ShieldCollision->SetRelativeLocation(FVector(2.7f, 8.2f, 9.0f));
	ShieldCollision->SetRelativeRotation(FRotator(0, -20.0f, 0));
	ShieldCollision->SetRelativeScale3D(FVector(1.25f, 2.2f, 0.8f));
	ShieldCollision->SetCollisionProfileName(TEXT("OverlapAll"));
	ShieldCollision->bHiddenInGame = true;
	ShieldCollision->ComponentTags.Add(FName(TEXT("PlayerShield")));

	// 스테이트바 씬의 위기값과 회전값 설정
	StateBarScene->SetRelativeLocation(FVector(-6.9f, -23.0f, 9.4f));
	StateBarScene->SetRelativeRotation(FRotator(0.0f, -19.0f, -90.0f));

	ShieldSoundComponent = CreateDefaultSubobject<UAudioComponent>(TEXT("SwordSoundComponent"));
	ShieldSoundComponent->SetupAttachment(ShieldMesh);
	//ShieldSoundComponent->SetRelativeLocation(FVector(0, 85.0f, 0));

	static ConstructorHelpers::FObjectFinder<USoundCue>PlayerShieldSoundAsset(TEXT("SoundCue'/Game/Assets/Sounds/Character/player_shield_Cue.player_shield_Cue'"));
	if (PlayerShieldSoundAsset.Succeeded())
	{
		ShieldSound = PlayerShieldSoundAsset.Object;
		ShieldSoundComponent->SetSound(ShieldSound);
	}

	IsActivation = false;
	IsMiniBossWeaponOverlap = false;
	IsBossWeaponOverlap = false;
	// 태그

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

	//방패를 쉴드 씬 컴포넌트에 스폰시킨다.
	StateBar = GetWorld()->SpawnActor<AHPStaminaBar_2>(StateBar->StaticClass(), StateBarScene->GetComponentLocation(), StateBarScene->GetComponentRotation(), SpawnActorOption);
	//방패를 AttachRules를 토대로 쉴드 씬 컴포넌트에 붙인다.
	StateBar->AttachToComponent(StateBarScene, AttachRules);
	// 오너 설정
	ShieldOwner = Cast<AMotionControllerCharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));

	if (ShieldCollision)
	{
		ShieldCollision->OnComponentBeginOverlap.AddDynamic(this, &APlayerShield::OnShieldOverlapStart);
		ShieldCollision->OnComponentEndOverlap.AddDynamic(this, &APlayerShield::OnShieldOverlapEnd);
	}
}

// Called every frame
void APlayerShield::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (ShieldOwner)
	{
		ShieldCurrentPosition = ShieldCollision->GetComponentLocation() - GetActorLocation();
		ShieldMoveDelta = ShieldCurrentPosition - ShieldPreviousPosition;
		ShieldMoveVelocity = ShieldMoveDelta / DeltaTime;
		ShieldPreviousPosition = ShieldCurrentPosition;
	}

	//GLog->Log(FString::Printf(TEXT("%d"), IsActivation));
}

void APlayerShield::ConvertOfOpacity(float opacity)		// Opacity값 세팅(캐릭터에서 호출)
{
	if (ShieldMesh)
	{
		ShieldMesh->SetScalarParameterValueOnMaterials(FName(TEXT("ShieldOpacity")), opacity);
	}

	if (IsActivation)
	{
		ShieldMesh->SetVectorParameterValueOnMaterials(TEXT("fresnel_color"), FVector(0, 0, 0));
		IsActivation = false;
	}
	else
	{
		ShieldMesh->SetVectorParameterValueOnMaterials(TEXT("fresnel_color"), FVector(1.0f, 0, 0.39f));
		IsActivation = true;
	}
}

void APlayerShield::OnShieldOverlapStart(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult)
{
	if (OtherComp->ComponentHasTag(TEXT("MBWeapon"))) //방패에 부딪힌 컴포넌트가 중간보스 무기인지 확인한다.
	{
		IsMiniBossWeaponOverlap = true; //중간보스 검이 겹쳐져 있다고 알려주고
		AMiniBossWeapon* MiniBossWeapon = Cast<AMiniBossWeapon>(OtherComp->GetOwner()); //부딪힌 컴포넌트를 소유하는 액터를 가져와서 중간보스 무기로 변환한다.

		if (MiniBossWeapon) //변환이 성공하고
		{
			if (IsActivation) //방패가 활성화 상태이고
			{
				if (MiniBossWeapon->IsWeaponAttack) //중간보스 검이 공격가능 상태이면
				{
					if (ShieldOwner) 
					{
						if (ShieldOwner->UseStamina(MiniBossWeapon->GetDamage()*1.2f)) //스테미너가 부족하지 않다면
						{
							if (!ShieldSoundComponent->IsPlaying())
							{
								ShieldSoundComponent->Play(0); //방패 막는 소리를 재생시키고
							}

							MiniBossWeapon->IsWeaponAttack = false; //공격을 불가능으로 만든다.
							UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), ShieldBlockEffect, OtherComp->GetComponentLocation()); //방패 막는 이펙트를 호출한다.
						}
					}
				}
			}						

			if (MiniBossWeapon->IsParryingAttack) //무기가 패링 가능한 상태인지 확인한다.
			{
				//패링가능한 상태라면 MiniBoss를 가지고 와서 저장하고
				AMiniBoss* MiniBoss = Cast<AMiniBoss>(MiniBossWeapon->GetAttachParentActor());

				if (MiniBoss)  //MiniBoss 형변환에 성공하면
				{
					if (IsActivation && IsMiniBossWeaponOverlap&& ShieldMoveVelocity.Size() > 90.0f) //그립버튼을 누르고 방패의 선속도가 90이상인지 확인한다.
					{
						// 최소 스테미너이상 있을 때 방패로 막기 가능 
						if (ShieldOwner->CurrentStamina > ShieldOwner->DefencePoint)
						{
							GLog->Log(FString::Printf(TEXT("중간보스 패링 진입")));
							UGameplayStatics::SetGlobalTimeDilation(GetWorld(), 0.5f);
							GetWorld()->GetTimerManager().SetTimer(GlobalTimeTimer, this, &APlayerShield::GlobalTimeInit, 0.5f, false);
							ShieldOwner->UseStamina(ShieldOwner->DefencePoint);		// 스테미너 감소 및 대기시간 후 스테미너 회복시작
							RumbleLeftController(1.0f); //패드에 진동을 울려주고
							MiniBossWeapon->IsParryingAttack = false;
							UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), ParryingEffect, OtherComp->GetComponentLocation());
							MiniBoss->ParryingManager->ParryingPointSet(MiniBoss, MiniBoss->ParryingPoints, MiniBoss->CurrentHP, MiniBoss->MaxHP, MiniBoss->ParryingPointMaxCount);							
							MiniBoss->CurrentParryingState = EMiniBossParryingState::ParryingStart;
							MiniBoss->CurrentAttackState = EMiniBossAttackState::ParryingState;
						}
					}
				}
			}
		}
	}
	else if (OtherComp->ComponentHasTag(TEXT("BossWeapon")))
	{	
		IsBossWeaponOverlap = true;

		ABossWeapon* BossWeapon = Cast<ABossWeapon>(OtherComp->GetOwner());
		if (BossWeapon)
		{
			if (IsActivation)
			{
				if (BossWeapon->IsWeaponAttack)
				{
					if (ShieldOwner)
					{
						if (ShieldOwner->UseStamina(BossWeapon->GetDamage()*1.2f))
						{
							if (!ShieldSoundComponent->IsPlaying())
							{
								ShieldSoundComponent->Play(0);
							}

							BossWeapon->IsWeaponAttack = false;
							UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), ShieldBlockEffect, OtherComp->GetComponentLocation());
						}
					}
				}
			}			

			if (BossWeapon->IsParryingAttack)
			{
				ABoss* Boss = Cast<ABoss>(BossWeapon->GetAttachParentActor());

				if (Boss)
				{
					if (IsActivation && IsBossWeaponOverlap && ShieldMoveVelocity.Size() > 90.0f)
					{
						if (ShieldOwner->UseStamina(ShieldOwner->DefencePoint))
						{
							UGameplayStatics::SetGlobalTimeDilation(GetWorld(), 0.5f);
							GetWorld()->GetTimerManager().SetTimer(GlobalTimeTimer, this, &APlayerShield::GlobalTimeInit, 0.5f, false);
							RumbleLeftController(1.0f);
							BossWeapon->IsParryingAttack = false;
							UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), ParryingEffect, OtherComp->GetComponentLocation());
							Boss->ParryingPointSet();
							Boss->CurrentParryingState = EBossParryingState::ParryingStart;
							Boss->CurrentCloseAttackState = EBossCloseAttackState::ParryingState;
						}						
					}
				}
			}
		}
	}
	else if (OtherComp->ComponentHasTag(TEXT("NormalMonsterWeapon")))
	{
		if (IsActivation)
		{
			ANMWeaponSword* NMSword = Cast<ANMWeaponSword>(OtherComp->GetOwner());

			if (NMSword)
			{
				if (NMSword->IsWeaponAttack)
				{
					if (ShieldOwner)
					{
						if (ShieldOwner->UseStamina(NMSword->GetDamage()*1.2f))
						{
							if (!ShieldSoundComponent->IsPlaying())
							{
								ShieldSoundComponent->Play(0);
							}

							RumbleLeftController(0.5f);

							NMSword->IsWeaponAttack = false;
							UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), ShieldBlockEffect, OtherComp->GetComponentLocation());
						}
					}
				}
			}
		}
	}
	else if (OtherComp->ComponentHasTag(TEXT("BossOrbWave")))
	{
		if (IsActivation)
		{
			ABossOrbWave* DefaultOrb = Cast<ABossOrbWave>(OtherComp->GetOwner());

			if (DefaultOrb)
			{
				if (ShieldOwner)
				{
					if (ShieldOwner->UseStamina(DefaultOrb->GetOrbDamage()*1.2f))
					{
						if (!ShieldSoundComponent->IsPlaying())
						{
							ShieldSoundComponent->Play(0);
						}

						RumbleLeftController(0.5f);

						UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), ShieldBlockEffect, DefaultOrb->GetActorLocation());
						DefaultOrb->Destroy();
					}
				}
			}
		}
	}
	else if (OtherComp->ComponentHasTag(TEXT("BossBlueOrbWave")))
	{
		if (IsActivation)
		{
			ABossBlueOrbWave* BossBlueOrbWave = Cast<ABossBlueOrbWave>(OtherComp->GetOwner());

			if (BossBlueOrbWave)
			{
				if (ShieldOwner)
				{
					if (ShieldOwner->UseStamina(BossBlueOrbWave->GetOrbDamage()*1.2f))
					{
						if (!ShieldSoundComponent->IsPlaying())
						{
							ShieldSoundComponent->Play(0);
						}

						RumbleLeftController(0.5f);

						UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), ShieldBlockEffect, BossBlueOrbWave->GetActorLocation());
						BossBlueOrbWave->Destroy();
					}
				}
			}			
			
			GetWorld()->GetTimerManager().SetTimer(ShieldFresnelTimer, this, &APlayerShield::Fresnel, 0.1f, true, 0.1f);
		}
	}
	else if (OtherComp->ComponentHasTag(TEXT("AttackBall")))
	{
		if (IsActivation)
		{
			if (ShieldOwner)
			{
				ABossAddAttackBall* AttackBall = Cast<ABossAddAttackBall>(OtherComp->GetOwner());

				if (AttackBall)
				{
					GetWorld()->GetTimerManager().SetTimer(ShieldFresnelTimer, this, &APlayerShield::Fresnel, 0.1f, true, 0.1f);
										
					if (ShieldOwner->UseStamina(AttackBall->GetDamage()*1.2f))
					{
						if (!ShieldSoundComponent->IsPlaying())
						{
							ShieldSoundComponent->Play(0);
						}

						RumbleLeftController(0.5f);

						UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), ShieldBlockEffect, AttackBall->GetActorLocation());
						AttackBall->Destroy();
					}					
				}
			}			
		}
	}
	else if (OtherComp->ComponentHasTag(TEXT("NMArrow")))
	{
		if (IsActivation)
		{
			ANMWeaponArrow* NMArrow = Cast<ANMWeaponArrow>(OtherComp->GetOwner());

			if (NMArrow)
			{
				if (ShieldOwner)
				{
					if (ShieldOwner->UseStamina(NMArrow->GetDamage()*1.2f))
					{
						if (!ShieldSoundComponent->IsPlaying())
						{
							ShieldSoundComponent->Play(0);
						}

						RumbleLeftController(0.5f);

						UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), ShieldBlockEffect, OtherComp->GetComponentLocation());
						NMArrow->Destroy();
					}
				}
			}
		}
	}
	else if (OtherComp->ComponentHasTag(TEXT("MiniBossSwordWave")))
	{
		if (IsActivation)
		{
			AMiniBossSwordWave* MBSwordWave = Cast<AMiniBossSwordWave>(OtherComp->GetOwner());

			if (MBSwordWave)
			{
				if (ShieldOwner)
				{
					if (ShieldOwner->UseStamina(MBSwordWave->GetDamage()*1.2f))
					{
						if (!ShieldSoundComponent->IsPlaying())
						{
							ShieldSoundComponent->Play(0);
						}

						RumbleLeftController(0.5f);

						UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), ShieldBlockEffect, OtherComp->GetComponentLocation());
						MBSwordWave->Destroy();
					}
				}
			}
		}
	}	
}

void APlayerShield::OnShieldOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (OtherComp->ComponentHasTag(TEXT("MiniBossWeaponCollision")))
	{
		IsMiniBossWeaponOverlap = false;
	}
	else if (OtherComp->ComponentHasTag(TEXT("BossWeaponCollision")))
	{
		IsBossWeaponOverlap = false;
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
			PC->RumbleHandController(LeftHand->Hand, Intensity);
		}
	}
}

void APlayerShield::GlobalTimeInit()
{
	UGameplayStatics::SetGlobalTimeDilation(GetWorld(), 1.0f);
	GLog->Log(FString::Printf(TEXT("타이머 도는중")));
}

void APlayerShield::Fresnel()
{
	//ShieldMesh()->

}