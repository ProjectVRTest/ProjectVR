﻿// Fill out your copyright notice in the Description page of Project Settings.

#include "Boss.h"
#include "BossAIController.h"
#include "Components/SkeletalMeshComponent.h"
#include "UObject/ConstructorHelpers.h"
#include "Perception/PawnSensingComponent.h"
#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Animation/AnimBlueprint.h"
#include "Weapon/BossWeapon.h"
#include "MyCharacter/MotionControllerCharacter.h"
#include "MyCharacter/CameraLocation.h"
#include "Components/CapsuleComponent.h"
#include "Components/SphereComponent.h"
#include "Monster/Boss/AI/AddAttack/BossAddAttackBall.h"
#include "Particles/ParticleSystem.h"
#include "Particles/ParticleSystemComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Components/BoxComponent.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Kismet/KismetMathLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "Monster/MiniBoss/MiniBossParryingPoint.h"
#include "MyTargetPoint.h"
#include "TimerManager.h"
#include "Component/Monster/MonsterParryingManager.h"

// Sets default values
ABoss::ABoss()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	GetCapsuleComponent()->SetCollisionProfileName(FName(TEXT("Monster")));
	
	AIControllerClass = ABossAIController::StaticClass();

	static ConstructorHelpers::FObjectFinder<USkeletalMesh>Boss_SK_Mesh(TEXT("SkeletalMesh'/Game/Assets/CharacterEquipment/Monster/Boss/Mesh/ES_Boss_fin.ES_Boss_fin'"));

	if (Boss_SK_Mesh.Succeeded())
	{
		GetMesh()->SetSkeletalMesh(Boss_SK_Mesh.Object);
	}

	GetCapsuleComponent()->bHiddenInGame = true;

	GetMesh()->SetRelativeLocation(FVector(0, 0, -88.0f));
	GetMesh()->SetRelativeRotation(FRotator(0, -90.0f, 0));

	CurrentState = EBossState::Idle;
	CurrentBattleState = EBossBattleState::Idle;
	CurrentBlinkAttackState = EBossBlinkAttackState::Idle;
	CurrentLongAttackState = EBossLongAttackState::Idle;
	CurrentCloseAttackState = EBossCloseAttackState::Idle;
	CurrentParryingState = EBossParryingState::Idle;
	CurrentBattleWatchState = EBossBattleWatchState::Idle;
	CurrentUltimateAttackState = EBossUltimateAttackState::Idle;
	CurrentWaveAttackState = EBossWaveAttackState::Idle;
	CurrentUpDownAttackState = EBossUpDownAttackState::Idle;

	PawnSensing = CreateDefaultSubobject<UPawnSensingComponent>(TEXT("PawnSensing"));
	PawnSensing->HearingThreshold = 2300.0f;
	PawnSensing->LOSHearingThreshold = 2400.0f;
	PawnSensing->bHearNoises = true;
	PawnSensing->bSeePawns = true;
	PawnSensing->SetPeripheralVisionAngle(40.0f);
	PawnSensing->SightRadius = 2200.0f;
	PawnSensing->SensingInterval = 0.01f;

	
	static ConstructorHelpers::FObjectFinder<UBehaviorTree>Boss_BT(TEXT("BehaviorTree'/Game/Blueprints/Monster/Boss/AI/BT_Boss.BT_Boss'"));

	if (Boss_BT.Succeeded())
	{
		BehaviorTree = Boss_BT.Object;
	}

	static ConstructorHelpers::FObjectFinder<UClass>ABP_Boss(TEXT("AnimBlueprint'/Game/Blueprints/Monster/Boss/Blueprints/ABP_Boss_Fin.ABP_Boss_Fin_C'"));

	if (ABP_Boss.Succeeded())
	{
		UClass* BossAnimBlueprint = ABP_Boss.Object;

		if (BossAnimBlueprint)
		{
			GetMesh()->SetAnimationMode(EAnimationMode::AnimationBlueprint);
			GetMesh()->SetAnimInstanceClass(BossAnimBlueprint);
		}
	}

	SwordWaveSpawn = CreateDefaultSubobject<USceneComponent>(TEXT("SwordWaveSpawn"));
	SwordWaveSpawn->SetupAttachment(GetRootComponent());
	SwordWaveSpawn->SetRelativeLocation(FVector(150.0f,0, 0));

	WaveSpawn = CreateDefaultSubobject<USceneComponent>(TEXT("WaveSpawn"));
	WaveSpawn->SetupAttachment(GetRootComponent());
	WaveSpawn->SetRelativeLocation(FVector(200.0f, 0, 200.0f));

	static ConstructorHelpers::FObjectFinder<UMaterialInterface> M_Opacity(TEXT("Material'/Game/Assets/CharacterEquipment/Monster/M_MonsterOpacity.M_MonsterOpacity'"));
	if (M_Opacity.Succeeded())
	{
		OpacityMaterials = M_Opacity.Object;
	}

	static ConstructorHelpers::FObjectFinder<UMaterialInterface> M_DefaultBodyMaterials(TEXT("MaterialInstanceConstant'/Game/Assets/CharacterEquipment/Monster/Boss/Materials/M_Boss_Inst.M_Boss_Inst'"));
	if (M_DefaultBodyMaterials.Succeeded())
	{
		DefaultBodyMaterials = M_DefaultBodyMaterials.Object;
	}

	static ConstructorHelpers::FObjectFinder<UMaterialInterface> M_DefaultClothMaterials(TEXT("MaterialInstanceConstant'/Game/Assets/CharacterEquipment/Monster/Boss/Materials/M_Boss_Cloth_Inst.M_Boss_Cloth_Inst'"));
	if (M_DefaultClothMaterials.Succeeded())
	{
		DefaultClothMaterials = M_DefaultClothMaterials.Object;
	}

	LeftCrashLocation = CreateDefaultSubobject<USceneComponent>(TEXT("LeftCrashLocation"));
	LeftCrashLocation->SetupAttachment(GetRootComponent());
	LeftCrashLocation->SetRelativeLocation(FVector(400.0f, -250.0f, 0));

	RightCrashLocation = CreateDefaultSubobject<USceneComponent>(TEXT("RightCrashLocation"));
	RightCrashLocation->SetupAttachment(GetRootComponent());
	RightCrashLocation->SetRelativeLocation(FVector(400.0f, 250.0f, 0));

	OrbCreateLocation = CreateDefaultSubobject<USceneComponent>(TEXT("OrbCreateLocation"));
	OrbCreateLocation->SetupAttachment(GetRootComponent());
	OrbCreateLocation->SetRelativeLocation(FVector(92.0f, -6.0f, 40.0f));

	ManyOrbBound = CreateDefaultSubobject<UBoxComponent>(TEXT("ManyOrbBound"));
	ManyOrbBound->SetupAttachment(GetRootComponent());
	ManyOrbBound->SetCollisionProfileName("NoCollision");
	ManyOrbBound->SetRelativeLocation(FVector(-142.0f,1.0f,318.0f));
	ManyOrbBound->SetRelativeScale3D(FVector(1.5f, 16.0f, 12.75f));
		
	UltimateAuraEffectComponent = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("UltimateAuraEffectComponent"));
	UltimateAuraEffectComponent->SetupAttachment(GetRootComponent());
	UltimateAuraEffectComponent->SetRelativeLocation(FVector(0, 0, -200.0f));

	ParryingManager = CreateDefaultSubobject<UMonsterParryingManager>(TEXT("ParryingManager"));

	MaxHP = 350.0f;
	CurrentHP = MaxHP;

	static ConstructorHelpers::FObjectFinder<UParticleSystem>PT_BlinkSmoke(TEXT("ParticleSystem'/Game/Assets/Effect/ES_Skill/PT_BossBlinkSmoke.PT_BossBlinkSmoke'"));
	if (PT_BlinkSmoke.Succeeded())
	{
		BlinkSmoke = PT_BlinkSmoke.Object;
	}

	static ConstructorHelpers::FObjectFinder<UParticleSystem>PT_UltimateAura(TEXT("ParticleSystem'/Game/Assets/Effect/ES_Skill/Boss_UltimateAura.Boss_UltimateAura'"));
	if (PT_UltimateAura.Succeeded())
	{
		UltimateAura = PT_UltimateAura.Object;
		UltimateAuraEffectComponent->Template = UltimateAura;
		UltimateAuraEffectComponent->SetVisibility(false);		
	}

	Target = nullptr;
	TargetCamera = nullptr;

	OrbMaxCount = 3;
	ParryingPointMaxCount = 0;
	ParryingPointCount = 0;

	UltimateOrbColor.SetUltimateNormalMonsterSpawnMaxCount(3);
	UltimateOrbColor.SetUltimateOrbMaxCount(15);
	CurrentNormalMonsterCount = UltimateOrbColor.GetUltimateNormalMonsterSpawnMaxCount();
	GetCharacterMovement()->MaxWalkSpeed = 250.0f;

	Tags.Add(TEXT("Monster"));
	Tags.Add(FName(TEXT("DisregardForLeftHand")));
	Tags.Add(FName(TEXT("DisregardForRightHand")));
}

// Called when the game starts or when spawned
void ABoss::BeginPlay()
{
	Super::BeginPlay();
	
	GetWorld()->GetTimerManager().SetTimer(BeginCharacterTimer, this, &ABoss::FindCharacter, 1.0f, false);		// 1.5초 후 무적시간을 비활성화

	ParryingManager->ParryingPointInit(this, ParryingPoints);

	//ParryingPointInit();
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), AMyTargetPoint::StaticClass(), TeleportPoints);
	
	for (int i = 0; i < TeleportPoints.Num(); i++)
	{
		if (TeleportPoints[i]->GetName() == TEXT("BossMapCenterPosition"))
		{
			MapCenterLocation = TeleportPoints[i]->GetActorLocation();
		}
		else
		{
			UltimateStartLocation = TeleportPoints[i]->GetActorLocation();
		}
	}

	FActorSpawnParameters SpawnActorOption;
	SpawnActorOption.Owner = this;
	SpawnActorOption.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

	FAttachmentTransformRules AttachRules(EAttachmentRule::SnapToTarget, EAttachmentRule::SnapToTarget, EAttachmentRule::KeepWorld, false);

	Sickle = GetWorld()->SpawnActor<ABossWeapon>(Sickle->StaticClass(), SpawnActorOption);

	if (Sickle)
	{
		Sickle->AttachToComponent(GetMesh(), AttachRules, FName(TEXT("BossWeaponSocket")));
	}

	if (PawnSensing)
	{
		PawnSensing->OnSeePawn.AddDynamic(this, &ABoss::OnSeeCharacter);
	}

	UltimateNormalMonsterSPawnPosition.Add(FVector(6081.0f, -732.0f, 211.0f));
	UltimateNormalMonsterSPawnPosition.Add(FVector(6263.0f, -265.0f, 248.0f));
	UltimateNormalMonsterSPawnPosition.Add(FVector(6109.0f, 531.0f, 211.0f));
}

// Called every frame
void ABoss::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	ABossAIController* AI = Cast<ABossAIController>(GetController());

	//GLog->Log(FString::Printf(TEXT("%d"), OrbMaxCount));

	//GLog->Log(FString::Printf(TEXT("UltimateOrbColor Red : %d Blue : %d Yellow : %d %d"), UltimateOrbColor.RedOrbColor, UltimateOrbColor.BlueOrbColor, UltimateOrbColor.YellowOrbColor, UltimateOrbColor.UltimateOrbMaxCount));

	if (AI)
	{
		AI->BBComponent->SetValueAsEnum("CurrentState", (uint8)CurrentState);
		AI->BBComponent->SetValueAsEnum("CurrentBlinkAttackState", (uint8)CurrentBlinkAttackState);
		AI->BBComponent->SetValueAsEnum("CurrentBattleState", (uint8)CurrentBattleState);
		AI->BBComponent->SetValueAsEnum("CurrentLongAttackState", (uint8)CurrentLongAttackState);
		AI->BBComponent->SetValueAsEnum("CurrentCloseAttackState", (uint8)CurrentCloseAttackState);
		AI->BBComponent->SetValueAsEnum("CurrentParryingState", (uint8)CurrentParryingState);
		AI->BBComponent->SetValueAsEnum("CurrentBattleWatchState", (uint8)CurrentBattleWatchState);
		AI->BBComponent->SetValueAsEnum("CurrentConfrontationState", (uint8)CurrentConfrontationState);
		AI->BBComponent->SetValueAsEnum("CurrentUltimateAttackState", (uint8)CurrentUltimateAttackState);
		AI->BBComponent->SetValueAsEnum("CurrentWaveAttackState", (uint8)CurrentWaveAttackState);
		AI->BBComponent->SetValueAsEnum("CurrentUpDownAttackState", (uint8)CurrentUpDownAttackState);
	} 
}

// Called to bind functionality to input
void ABoss::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

void ABoss::ParryingPointInit()
{
	ParryingPoints.Add(TEXT("HeadParryingPoint"));
	ParryingPoints.Add(TEXT("RightHandParryingPoint"));
	ParryingPoints.Add(TEXT("LeftHandParryingPoint"));
	ParryingPoints.Add(TEXT("LeftHandFingerParryingPoint"));
	ParryingPoints.Add(TEXT("SpineParryingPoint"));
	ParryingPoints.Add(TEXT("LeftKneeParryingPoint"));
	ParryingPoints.Add(TEXT("RightKneeParryingPoint"));
}

void ABoss::ParryingPointSet()
{
	float HPPercent = CurrentHP / MaxHP;
	int PreviousParryingPointName = -1;
	int RandomParryingPointName;

	AMiniBossParryingPoint* MiniBossParryingPoint;

	FActorSpawnParameters SpawnActorOption;
	SpawnActorOption.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

	FAttachmentTransformRules AttachRules(EAttachmentRule::SnapToTarget, EAttachmentRule::SnapToTarget, EAttachmentRule::KeepWorld, false);

	if (HPPercent > 0.5f && HPPercent <= 1.0f)
	{
		int RandomParryingPointSpawn = FMath::RandRange(1, 2);

		ParryingPointMaxCount = RandomParryingPointSpawn;

		for (int i = 0; i < ParryingPointMaxCount; i++)
		{
			RandomParryingPointName = FMath::RandRange(0, 6);

			if (RandomParryingPointName == PreviousParryingPointName)
			{
				i--;
				continue;
			}

			MiniBossParryingPoint = GetWorld()->SpawnActor<AMiniBossParryingPoint>(MiniBossParryingPoint->StaticClass(), GetActorLocation(), GetActorRotation(), SpawnActorOption);

			FName ParryingPointSpawnLocation = ParryingPoints[RandomParryingPointName];
			
			MiniBossParryingPoint->AttachToComponent(GetMesh(), AttachRules, ParryingPointSpawnLocation);

			PreviousParryingPointName = RandomParryingPointName;
		}
	}
	else if (HPPercent > 0.4f &&HPPercent <= 0.5f)
	{
		ParryingPointValueSet(3); //3개 랜덤하게 스폰
	}
	else if (HPPercent > 0.3f && HPPercent <= 0.4f)
	{
		ParryingPointValueSet(5); //5개 랜덤하게 스폰
	}
	else if (HPPercent >= 0 && HPPercent <= 0.3f)
	{
		ParryingPointValueSet(7); //7개 스폰
	}

}

void ABoss::ParryingPointValueSet(int ParryingCount)
{
	int RandomParryingPointName;
	ParryingPointMaxCount = ParryingCount;
	TArray<int32> RandomPointNotOverlap;
	bool RandomFlag;

	for (int k = 0; k < ParryingCount; k++)
	{
		RandomPointNotOverlap.Add(-1);
	}

	AMiniBossParryingPoint * MiniBossParryingPoint;
	FActorSpawnParameters SpawnActorOption;
	SpawnActorOption.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

	FAttachmentTransformRules AttachRules(EAttachmentRule::SnapToTarget, EAttachmentRule::SnapToTarget, EAttachmentRule::KeepWorld, false);

	for (int i = 0; i < ParryingCount; i++)
	{
		RandomParryingPointName = FMath::RandRange(0, 6);
		RandomFlag = true;

		for (int j = 0; j < ParryingCount; j++)
		{
			if (RandomPointNotOverlap[j] == RandomParryingPointName)
			{
				i--;
				RandomFlag = false;
			}
		}

		if (RandomFlag)
		{
			RandomPointNotOverlap[i] = RandomParryingPointName;
		}
	}

	for (int j = 0; j < ParryingCount; j++)
	{
		MiniBossParryingPoint = GetWorld()->SpawnActor<AMiniBossParryingPoint>(MiniBossParryingPoint->StaticClass(), GetActorLocation(), GetActorRotation(), SpawnActorOption);

		if (MiniBossParryingPoint)
		{
			FName ParryingPointSpawnLocation = ParryingPoints[RandomPointNotOverlap[j]];
			MiniBossParryingPoint->AttachToComponent(GetMesh(), AttachRules, ParryingPointSpawnLocation);
		}		
	}

	RandomPointNotOverlap.Empty(); //
}

void ABoss::OnSeeCharacter(APawn * Pawn)
{
	/*if (Pawn->ActorHasTag("Character"))
	{
		GLog->Log(FString::Printf(TEXT("캐릭터 발견")));
		ABossAIController* AI = Cast<ABossAIController>(GetController());

		if (AI)
		{
			if (!Target)
			{
				AMotionControllerCharacter* MyCharacter = Cast<AMotionControllerCharacter>(Pawn);

				if (MyCharacter)
				{
					Target = MyCharacter;
					TargetCamera = MyCharacter->CameraLocation;
					AI->BBComponent->SetValueAsObject("Player", Pawn);
					AI->BBComponent->SetValueAsObject("PlayerCamera", TargetCamera);
					CurrentBlinkAttackState = EBossBlinkAttackState::AddAttackStart;
					CurrentBattleState = EBossBattleState::AddAttack;
					CurrentState = EBossState::Battle;
				}
			}
		}
	}
	else
	{
		GLog->Log(FString::Printf(TEXT("캐릭터 노발견")));
	}*/
}

void ABoss::OnHearNoise(APawn * Pawn, const FVector & Location, float Volume)
{
	if (!Target)
	{
		if (Pawn->ActorHasTag(TEXT("Character")))
		{
			AMotionControllerCharacter* Mycharacter = Cast<AMotionControllerCharacter>(Pawn);

			if (Mycharacter)
			{
				Target = Mycharacter;
				if (Mycharacter->CameraLocation)
				{
					TargetCamera = Mycharacter->CameraLocation;
					ABossAIController* AI = Cast<ABossAIController>(GetController());
					if (AI)
					{
						AI->BBComponent->SetValueAsObject("Player", Pawn);
						AI->BBComponent->SetValueAsObject("PlayerCamera", TargetCamera);

						FRotator LookRotator = FRotationMatrix::MakeFromX(Pawn->GetActorLocation() - GetActorLocation()).Rotator();
						SetActorRotation(LookRotator);

						CurrentBlinkAttackState = EBossBlinkAttackState::AddAttackStart;
						CurrentBattleState = EBossBattleState::AddAttack;
						CurrentState = EBossState::Battle;
					}
				}
			}
		}
	}
}

float ABoss::TakeDamage(float Damage, FDamageEvent const & DamageEvent, AController * EventInstigator, AActor * DamageCauser)
{
	Super::TakeDamage(Damage, DamageEvent, EventInstigator, DamageCauser);

	GLog->Log(FString::Printf(TEXT("보스를 때림")));

	CurrentHP -= Damage;

	if (CurrentHP < 0)
	{
		CurrentHP = 0;

		if (Sickle)
		{
			Sickle->Destroy();
		}
		GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_Walking);
		CurrentState = EBossState::Dead;
	}

	float HPPercent = CurrentHP / MaxHP;

	GLog->Log(FString::Printf(TEXT("HPPercent %f"), HPPercent));
	
	if (CurrentState != EBossState::Dead)
	{
		if (HPPercent >= 0.70f && HPPercent <= 0.71f)
		{
			CurrentBlinkAttackState = EBossBlinkAttackState::Idle;
			CurrentLongAttackState = EBossLongAttackState::Idle;
			CurrentCloseAttackState = EBossCloseAttackState::Idle;
			CurrentParryingState = EBossParryingState::Idle;
			CurrentBattleWatchState = EBossBattleWatchState::Idle;
			CurrentConfrontationState = EBossConfrontationState::Idle;
			CurrentUltimateAttackState = EBossUltimateAttackState::Idle;
			CurrentBattleState = EBossBattleState::UltimateAttack;
		}
		else if (HPPercent >= 0.40f && HPPercent <= 0.41f)
		{
			CurrentBlinkAttackState = EBossBlinkAttackState::Idle;
			CurrentLongAttackState = EBossLongAttackState::Idle;
			CurrentCloseAttackState = EBossCloseAttackState::Idle;
			CurrentParryingState = EBossParryingState::Idle;
			CurrentBattleWatchState = EBossBattleWatchState::Idle;
			CurrentConfrontationState = EBossConfrontationState::Idle;
			CurrentUltimateAttackState = EBossUltimateAttackState::Idle;
			CurrentBattleState = EBossBattleState::UltimateAttack;
		}
		else if (HPPercent >= 0.10f && HPPercent <= 0.12f)
		{
			CurrentBlinkAttackState = EBossBlinkAttackState::Idle;
			CurrentLongAttackState = EBossLongAttackState::Idle;
			CurrentCloseAttackState = EBossCloseAttackState::Idle;
			CurrentParryingState = EBossParryingState::Idle;
			CurrentBattleWatchState = EBossBattleWatchState::Idle;
			CurrentConfrontationState = EBossConfrontationState::Idle;
			CurrentUltimateAttackState = EBossUltimateAttackState::Idle;
			CurrentBattleState = EBossBattleState::UltimateAttack;
		}
	}	

	return Damage;
}

void ABoss::FindCharacter()
{
	ABossAIController* AI = Cast<ABossAIController>(GetController());
	if (AI)
	{
		if (!Target)
		{
			AMotionControllerCharacter* MyCharacter = Cast<AMotionControllerCharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));

			if (MyCharacter)
			{
				Target = MyCharacter;
				TargetCamera = MyCharacter->CameraLocation;
				AI->BBComponent->SetValueAsObject("Player", MyCharacter);
				AI->BBComponent->SetValueAsObject("PlayerCamera", TargetCamera);
				CurrentBlinkAttackState = EBossBlinkAttackState::AddAttackStart;
				CurrentBattleState = EBossBattleState::AddAttack;
				CurrentState = EBossState::Battle;
			}
		}
	}
}
