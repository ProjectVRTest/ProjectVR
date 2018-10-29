// Fill out your copyright notice in the Description page of Project Settings.

#include "NormalMonster.h"
#include "UObject/ConstructorHelpers.h"
#include "Components/SkeletalMeshComponent.h"

#include "Perception/PawnSensingComponent.h" 
#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BlackboardComponent.h"

#include "NormalMonsterAIController.h"

#include "Animation/AnimBlueprint.h"

#include "Kismet/GameplayStatics.h"
#include "GameFramework/CharacterMovementComponent.h"

#include "Weapon/Bow/NMWeaponBow.h"
#include "Weapon/Sword/NMWeaponSword.h"

#include "Components/StaticMeshComponent.h"
#include "Engine/StaticMesh.h"

#include "MyCharacter/CameraLocation.h"
#include "MyCharacter/MotionControllerCharacter.h"
#include "Components/CapsuleComponent.h"

// Sets default values
ANormalMonster::ANormalMonster()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	/*GetCapsuleComponent()->SetCollisionObjectType(ECollisionChannel::ECC_GameTraceChannel1);	*/
	static ConstructorHelpers::FObjectFinder<USkeletalMesh>Normal_Monster_SK_Mesh(TEXT("SkeletalMesh'/Game/Assets/CharacterEquipment/Monster/NormalMonster/Mesh/Character/SK_NormalMonster.SK_NormalMonster'"));

	if (Normal_Monster_SK_Mesh.Succeeded())
	{
		SwordSKMesh = Normal_Monster_SK_Mesh.Object;
		GetMesh()->SetSkeletalMesh(SwordSKMesh);
		GetMesh()->SetRelativeLocation(FVector(0, 0, -88.0f));
		GetMesh()->SetRelativeRotation(FRotator(0, -90.0f, 0));
	}	

	static ConstructorHelpers::FObjectFinder<USkeletalMesh>NM_Archer_SK_Mesh(TEXT("SkeletalMesh'/Game/Assets/CharacterEquipment/Monster/NormalMonster/Mesh/Character/SK_NormalMonsterTwo.SK_NormalMonsterTwo'"));

	if (NM_Archer_SK_Mesh.Succeeded())
	{
		ArcherSKMesh = NM_Archer_SK_Mesh.Object;
	}

	CurrentState = ENormalMonsterState::Idle;
	CurrentAnimState = ENormalMonsterAnimState::Wait;
	CurrentIdleState = ENormalMonsterIdleState::Wait;
	CurrentAttackState = ENormalMonsterAttackState::Idle;
	CurrentStabAttackState = ENormalMonsterStabAttackState::Idle;
	CurrentComboAttackState = ENormalMonsterComboAttackState::Idle;
	CurrentArcherAttackState = ENormalMonsterArcherAttackState::idle;

	static ConstructorHelpers::FObjectFinder<UAnimMontage>NMAttackReverse_Montage(TEXT("AnimMontage'/Game/Blueprints/Monster/Normal/SwordArcher/Animation/NM_Attack_Reverse.NM_Attack_Reverse'"));

	if (NMAttackReverse_Montage.Succeeded())
	{
		NMAttackReverseMontage = NMAttackReverse_Montage.Object;
	}

	PawnSensing = CreateDefaultSubobject<UPawnSensingComponent>(TEXT("PawnSensing"));
	PawnSensing->HearingThreshold = 1200.0f;
	PawnSensing->LOSHearingThreshold = 1400.0f;
	PawnSensing->bHearNoises = true;
	PawnSensing->bSeePawns = true;
	PawnSensing->SetPeripheralVisionAngle(30.0f);
	PawnSensing->SightRadius = 3000.0f;
	PawnSensing->SensingInterval = 0.1f;

	static ConstructorHelpers::FObjectFinder<UBehaviorTree>NormalSwordMonster_BT(TEXT("BehaviorTree'/Game/Blueprints/Monster/Normal/SwordArcher/AI/BT_NormalSwordMonster.BT_NormalSwordMonster'"));

	if (NormalSwordMonster_BT.Succeeded())
	{
		SwordBehaviorTree = NormalSwordMonster_BT.Object;
	}

	static ConstructorHelpers::FObjectFinder<UBehaviorTree>NormalMoveArcherMonster_BT(TEXT("BehaviorTree'/Game/Blueprints/Monster/Normal/SwordArcher/AI/BT_NormalMoveArcherMonster.BT_NormalMoveArcherMonster'"));

	if (NormalMoveArcherMonster_BT.Succeeded())
	{
		MoveArcherBehaviorTree = NormalMoveArcherMonster_BT.Object;
	}

	QuiverComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("QuiverComponent"));
	QuiverComponent->SetupAttachment(GetMesh(),TEXT("QuiverSocket"));
	QuiverComponent->SetCollisionProfileName(TEXT("OverlapAll"));

	static ConstructorHelpers::FObjectFinder<UStaticMesh>SM_Quiver(TEXT("StaticMesh'/Game/Assets/CharacterEquipment/Monster/NormalMonster/Mesh/Weapon/Bow/Arrow/Mesh/SM_Quiver.SM_Quiver'"));

	if (SM_Quiver.Succeeded())
	{
		QuiverMesh = SM_Quiver.Object;
	}

	NMArrowComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("NMArrowComponent"));
	NMArrowComponent->SetupAttachment(GetMesh(), TEXT("ArrowSocket"));
	NMArrowComponent->SetCollisionProfileName(TEXT("OverlapAll"));

	static ConstructorHelpers::FObjectFinder<UStaticMesh>SM_Arrow(TEXT("StaticMesh'/Game/Assets/CharacterEquipment/Monster/NormalMonster/Mesh/Weapon/Bow/Arrow/Mesh/SM_Arrow.SM_Arrow'"));

	if (SM_Arrow.Succeeded())
	{
		ArrowMesh = SM_Arrow.Object;
	}

	ArrowSpawnLocation = CreateDefaultSubobject<USceneComponent>(TEXT("ArrowSpawnLocation"));
	ArrowSpawnLocation->SetupAttachment(GetRootComponent());
	ArrowSpawnLocation->SetRelativeLocation(FVector(90.0f, 8.0f, 50.0f));
	ArrowSpawnLocation->SetRelativeScale3D(FVector(-8.0f,90.0f,134.0f));
	MaxHP = 5.0f;
	CurrentHP = MaxHP;

	AIControllerClass = ANormalMonsterAIController::StaticClass();

	static ConstructorHelpers::FObjectFinder<UClass>ABP_NormalMonster(TEXT("AnimBlueprint'/Game/Blueprints/Monster/Normal/SwordArcher/Blueprints/ABP_NormalMonster.ABP_NormalMonster_C'"));

	if (ABP_NormalMonster.Succeeded())
	{
		UClass* NormalMonsterAnimBlueprint = ABP_NormalMonster.Object;
		
		if (NormalMonsterAnimBlueprint)
		{
			GetMesh()->SetAnimationMode(EAnimationMode::AnimationBlueprint);
			GetMesh()->SetAnimInstanceClass(NormalMonsterAnimBlueprint);
		}		
	}
	GetCharacterMovement()->MaxWalkSpeed = 250.0f;
	Target = nullptr;

	Tags.Add(TEXT("Monster"));
	Tags.Add(FName(TEXT("DisregardForLeftHand")));
	Tags.Add(FName(TEXT("DisregardForRightHand")));
}

// Called when the game starts or when spawned
void ANormalMonster::BeginPlay()
{
	Super::BeginPlay();
	ANormalMonsterAIController* AI = Cast<ANormalMonsterAIController>(GetController());

	FActorSpawnParameters SpawnActorOption;
	SpawnActorOption.Owner = this;
	SpawnActorOption.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

	FAttachmentTransformRules AttachRules(EAttachmentRule::SnapToTarget, EAttachmentRule::SnapToTarget, EAttachmentRule::KeepWorld, false);

	int RandomMesh = FMath::RandRange(1, 2);

	if (RandomMesh == 1)
	{
		GetMesh()->SetSkeletalMesh(ArcherSKMesh);
	}

	if (AI)
	{
		AI->BBComponent->SetValueAsEnum(TEXT("MonsterKind"), (uint8)MonsterKind);
	}

	if (PawnSensing)
	{
		PawnSensing->OnSeePawn.AddDynamic(this, &ANormalMonster::OnSeeCharacter);
		PawnSensing->OnHearNoise.AddDynamic(this, &ANormalMonster::OnHearNoise);
	}

	switch (MonsterKind)
	{
	case ENormalMonsterKind::SwordMan:
		Sword = GetWorld()->SpawnActor<ANMWeaponSword>(Sword->StaticClass(), SpawnActorOption);
		if (Sword)
		{
			Sword->AttachToComponent(GetMesh(), AttachRules, TEXT("SwordSocket"));
		}
		break;
	case ENormalMonsterKind::MoveArcher:
		Bow = GetWorld()->SpawnActor<ANMWeaponBow>(Bow->StaticClass(), SpawnActorOption);
		if (Bow)
		{
			Bow->AttachToComponent(GetMesh(), AttachRules, TEXT("BowSocket"));
			if (QuiverMesh)
			{
				QuiverComponent->SetStaticMesh(QuiverMesh);
			}
		}		
		break;
	}	
	
}

// Called every frame
void ANormalMonster::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	ANormalMonsterAIController* AI = Cast<ANormalMonsterAIController>(GetController());

	if (AI)
	{
		AI->BBComponent->SetValueAsEnum("CurrentState", (uint8)CurrentState);
		AI->BBComponent->SetValueAsEnum("CurrentAnimState", (uint8)CurrentAnimState);
		AI->BBComponent->SetValueAsEnum("CurrentIdleState", (uint8)CurrentIdleState);
		AI->BBComponent->SetValueAsEnum("CurrentAttackState", (uint8)CurrentAttackState);
		AI->BBComponent->SetValueAsEnum("CurrentStabAttackState", (uint8)CurrentStabAttackState);
		AI->BBComponent->SetValueAsEnum("CurrentComboAttackState", (uint8)CurrentComboAttackState);
		AI->BBComponent->SetValueAsEnum("CurrentArcherAttackState", (uint8)CurrentArcherAttackState);
	}
}

// Called to bind functionality to input
void ANormalMonster::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}

void ANormalMonster::SpawnArrowMesh()
{
	NMArrowComponent->SetStaticMesh(ArrowMesh);
}

void ANormalMonster::DeleteArrowMesh()
{
	NMArrowComponent->SetStaticMesh(nullptr);
}

void ANormalMonster::OnSeeCharacter(APawn * Pawn)
{
	if (Pawn->ActorHasTag("Character"))
	{
		ANormalMonsterAIController* AI = Cast<ANormalMonsterAIController>(GetController());

		if (AI)
		{
			if (CurrentState == ENormalMonsterState::AttackWait)
			{
				return;
			}
			else
			{
				if (Target == nullptr)
				{
					AMotionControllerCharacter* MyCharacter = Cast<AMotionControllerCharacter>(Pawn);

					if (MyCharacter)
					{
						Target = Pawn;
						TargetCamera = MyCharacter->CameraLocation;
						AI->BBComponent->SetValueAsObject("Player", Pawn);
						AI->BBComponent->SetValueAsObject("PlayerCamera", TargetCamera);

						switch (MonsterKind)
						{
						case ENormalMonsterKind::SwordMan:
							CurrentAnimState = ENormalMonsterAnimState::Wait;
							CurrentState = ENormalMonsterState::Chase;
							break;
						case ENormalMonsterKind::MoveArcher:
							CurrentAnimState = ENormalMonsterAnimState::Walk;
							CurrentState = ENormalMonsterState::Chase;
							break;
						}
					}					
				}
			}
		}
	}
}

void ANormalMonster::OnHearNoise(APawn* Pawn, const FVector& Location, float Volume)
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
					ANormalMonsterAIController* AI = Cast<ANormalMonsterAIController>(GetController());
					if (AI)
					{
						AI->BBComponent->SetValueAsObject("Player", Pawn);
						AI->BBComponent->SetValueAsObject("PlayerCamera", TargetCamera);

						FRotator LookRotator = FRotationMatrix::MakeFromX(Pawn->GetActorLocation() - GetActorLocation()).Rotator();
						SetActorRotation(LookRotator);

						CurrentAnimState = ENormalMonsterAnimState::Walk;
						CurrentState = ENormalMonsterState::Chase;

					}
				}
			}
		}
	}	
}

float ANormalMonster::TakeDamage(float Damage, FDamageEvent const & DamageEvent, AController * EventInstigator, AActor * DamageCauser)
{
	Super::TakeDamage(Damage, DamageEvent, EventInstigator, DamageCauser);

	CurrentHP -= Damage;

	if (CurrentHP > 0)
	{
		GLog->Log(FString::Printf(TEXT("캐릭터로부터 데미지 받음")));
		

		ANormalMonsterAIController* AI = Cast<ANormalMonsterAIController>(GetController());

		if (CurrentState == ENormalMonsterState::AttackWait)
		{
			Target = DamageCauser;
			AI->BBComponent->SetValueAsObject("Player", DamageCauser);

			float Distance = AI->BBComponent->GetValueAsFloat(TEXT("Distnace"));

			if (Distance <= 400.0f)
			{
				CurrentState = ENormalMonsterState::Battle;
			}
			else
			{
				CurrentState = ENormalMonsterState::Chase;
				switch (MonsterKind)
				{
				case ENormalMonsterKind::SwordMan:
					CurrentAnimState = ENormalMonsterAnimState::Wait;
					break;
				case ENormalMonsterKind::MoveArcher:
					CurrentAnimState = ENormalMonsterAnimState::Walk;
					break;
				}
			}
		}
	}
	else if (CurrentHP < 0)
	{
		GetMesh()->SetCollisionProfileName(TEXT("NoCollision"));
		GLog->Log(FString::Printf(TEXT("HP가 0보다 작은데 공격 받음")));		
		CurrentHP = 0;
		CurrentState = ENormalMonsterState::Dead;
	}
	return Damage;
}

