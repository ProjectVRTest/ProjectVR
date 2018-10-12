// Fill out your copyright notice in the Description page of Project Settings.

#include "DontMoveArcher.h"
#include "UObject/ConstructorHelpers.h"
#include "Components/SkeletalMeshComponent.h"
#include "Perception/PawnSensingComponent.h" 
#include "Components/StaticMeshComponent.h"
#include "Engine/StaticMesh.h"
#include "DontMoveArcherAIController.h"
#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Animation/AnimBlueprint.h"
#include "Monster/Normal/DontMoveArcher/Weapon/Bow/DontMoveArcherBow.h"
#include "MyCharacter/MotionControllerCharacter.h"
#include "MyCharacter/CameraLocation.h"
#include "Kismet/KismetMathLibrary.h"

// Sets default values
ADontMoveArcher::ADontMoveArcher()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	CurrentState = EDontMoveArcherState::Idle;
	CurrentAnimState = EDontMoveArcherAnimState::Wait;
	CurrentAttackState = EDontMoveArcherAttackState::Idle;
	CurrentArcherAttackState = EDontMoveArcherArcherAttackState::Idle;

	static ConstructorHelpers::FObjectFinder<USkeletalMesh>NM_SK_DontMoveSword(TEXT("SkeletalMesh'/Game/Assets/CharacterEquipment/Monster/NormalMonster/Mesh/Character/SK_NormalMonster.SK_NormalMonster'"));

	if (NM_SK_DontMoveSword.Succeeded())
	{
		SwordSKMesh = NM_SK_DontMoveSword.Object;
		GetMesh()->SetSkeletalMesh(SwordSKMesh);
		GetMesh()->SetRelativeLocation(FVector(0, 0, -88.0f));
		GetMesh()->SetRelativeRotation(FRotator(0, -90.0f, 0));
	}

	static ConstructorHelpers::FObjectFinder<USkeletalMesh>NM_SK_DontMoveArcher(TEXT("SkeletalMesh'/Game/Assets/CharacterEquipment/Monster/NormalMonster/Mesh/Character/SK_NormalMonsterTwo.SK_NormalMonsterTwo'"));

	if (NM_SK_DontMoveArcher.Succeeded())
	{
		ArcherSKMesh = NM_SK_DontMoveArcher.Object;
	}

	PawnSensing = CreateDefaultSubobject<UPawnSensingComponent>(TEXT("PawnSensing"));
	PawnSensing->bHearNoises = false;
	PawnSensing->bSeePawns = true;
	PawnSensing->SetPeripheralVisionAngle(30.0f);
	PawnSensing->SightRadius = 3000.0f;
	PawnSensing->SensingInterval = 0.1f;

	QuiverComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("QuiverComponent"));
	QuiverComponent->SetupAttachment(GetMesh(), TEXT("QuiverSocket"));

	static ConstructorHelpers::FObjectFinder<UStaticMesh>SM_Quiver(TEXT("StaticMesh'/Game/Assets/CharacterEquipment/Monster/NormalMonster/Mesh/Weapon/Bow/Arrow/Mesh/SM_Quiver.SM_Quiver'"));

	if (SM_Quiver.Succeeded())
	{
		QuiverMesh = SM_Quiver.Object;
	}

	NMArrowComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("NMArrowComponent"));
	NMArrowComponent->SetupAttachment(GetMesh(), TEXT("ArrowSocket"));

	static ConstructorHelpers::FObjectFinder<UStaticMesh>SM_Arrow(TEXT("StaticMesh'/Game/Assets/CharacterEquipment/Monster/NormalMonster/Mesh/Weapon/Bow/Arrow/Mesh/SM_Arrow.SM_Arrow'"));

	if (SM_Arrow.Succeeded())
	{
		ArrowMesh = SM_Arrow.Object;
	}

	ArrowSpawnLocation = CreateDefaultSubobject<USceneComponent>(TEXT("ArrowSpawnLocation"));
	ArrowSpawnLocation->SetupAttachment(GetRootComponent());
	ArrowSpawnLocation->SetRelativeLocation(FVector(90.0f, 8.0f, 50.0f));
	ArrowSpawnLocation->SetRelativeScale3D(FVector(-8.0f, 90.0f, 134.0f));

	MaxHP = 100;
	CurrentHP = MaxHP;

	AIControllerClass = ADontMoveArcherAIController::StaticClass();

	static ConstructorHelpers::FObjectFinder<UClass>ABP_NormalMonster(TEXT("AnimBlueprint'/Game/Blueprints/Monster/Normal/DontMoveArcher/Blueprints/ABP_DontMoveArcher.ABP_DontMoveArcher_C'"));

	if (ABP_NormalMonster.Succeeded())
	{
		UClass* NormalMonsterAnimBlueprint = ABP_NormalMonster.Object;

		if (NormalMonsterAnimBlueprint)
		{
			GetMesh()->SetAnimationMode(EAnimationMode::AnimationBlueprint);
			GetMesh()->SetAnimInstanceClass(NormalMonsterAnimBlueprint);
		}
	}

	static ConstructorHelpers::FObjectFinder<UBehaviorTree>DontMoveArcher_BT(TEXT("BehaviorTree'/Game/Blueprints/Monster/Normal/DontMoveArcher/AI/BT_DontMoveArcher.BT_DontMoveArcher'"));

	if (DontMoveArcher_BT.Succeeded())
	{
		BehaviorTree = DontMoveArcher_BT.Object;
	}

	Target = nullptr;
	Pitch = 0;
	Tags.Add(TEXT("Monster"));
	Tags.Add(FName(TEXT("DisregardForLeftHand")));
	Tags.Add(FName(TEXT("DisregardForRightHand")));
}

// Called when the game starts or when spawned
void ADontMoveArcher::BeginPlay()
{
	Super::BeginPlay();
	
	FActorSpawnParameters SpawnActorOption;
	SpawnActorOption.Owner = this;
	SpawnActorOption.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

	FAttachmentTransformRules AttachRules(EAttachmentRule::SnapToTarget, EAttachmentRule::SnapToTarget, EAttachmentRule::KeepWorld, false);

	Bow = GetWorld()->SpawnActor<ADontMoveArcherBow>(Bow->StaticClass(), SpawnActorOption);

	int RandomMesh = FMath::RandRange(1, 2);

	if (RandomMesh == 1)
	{
		GetMesh()->SetSkeletalMesh(ArcherSKMesh);
	}

	if (PawnSensing)
	{
		PawnSensing->OnSeePawn.AddDynamic(this, &ADontMoveArcher::OnSeeCharacter);
	}

	if (Bow)
	{
		Bow->AttachToComponent(GetMesh(), AttachRules, TEXT("BowSocket"));
		if (QuiverMesh)
		{
			QuiverComponent->SetStaticMesh(QuiverMesh);
		}
	}
}

// Called every frame
void ADontMoveArcher::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	ADontMoveArcherAIController* AI = Cast<ADontMoveArcherAIController>(GetController());

	if (AI)
	{
		AI->BBComponent->SetValueAsEnum("CurrentState", (uint8)CurrentState);
		AI->BBComponent->SetValueAsEnum("CurrentAnimState", (uint8)CurrentAnimState);
		AI->BBComponent->SetValueAsEnum("CurrentAttackState", (uint8)CurrentAttackState);
		AI->BBComponent->SetValueAsEnum("CurrentArcherAttackState", (uint8)CurrentArcherAttackState);
		AI->BBComponent->SetValueAsObject("Player", Target);
		AI->BBComponent->SetValueAsObject("PlayerCamera", TargetCamera);
	}

	if (Target)
	{
		FRotator LookAt = UKismetMathLibrary::FindLookAtRotation(GetActorLocation(), Target->GetActorLocation());

		Pitch = LookAt.Pitch;
		GLog->Log(FString::Printf(TEXT("Pitch : %f"), Pitch));
	}
}

// Called to bind functionality to input
void ADontMoveArcher::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

void ADontMoveArcher::SpawnArrowMesh()
{
	NMArrowComponent->SetStaticMesh(ArrowMesh);
}

void ADontMoveArcher::DeleteArrowMesh()
{
	NMArrowComponent->SetStaticMesh(nullptr);
}

float ADontMoveArcher::TakeDamage(float Damage, FDamageEvent const & DamageEvent, AController * EventInstigator, AActor * DamageCauser)
{
	return Damage;
}

void ADontMoveArcher::OnSeeCharacter(APawn * Pawn)
{
	if (Pawn->ActorHasTag("Character"))
	{
		ADontMoveArcherAIController* AI = Cast<ADontMoveArcherAIController>(GetController());

		if (AI)
		{
			if (Target == nullptr)
			{
				AMotionControllerCharacter* MyCharacter = Cast<AMotionControllerCharacter>(Pawn);

				if (MyCharacter)
				{
					TargetCamera = MyCharacter->CameraLocation;
					AI->BBComponent->SetValueAsObject("PlayerCamera", TargetCamera);
				}
				
				Target = Pawn;
				AI->BBComponent->SetValueAsObject("Player", Pawn);		
			}
		}
	}
}

