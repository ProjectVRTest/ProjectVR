// Fill out your copyright notice in the Description page of Project Settings.

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

// Sets default values
ABoss::ABoss()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	AIControllerClass = ABossAIController::StaticClass();

	static ConstructorHelpers::FObjectFinder<USkeletalMesh>Boss_SK_Mesh(TEXT("SkeletalMesh'/Game/Assets/CharacterEquipment/Monster/Boss/Mesh/test_mesh.test_mesh'"));

	if (Boss_SK_Mesh.Succeeded())
	{
		GetMesh()->SetSkeletalMesh(Boss_SK_Mesh.Object);
	}

	GetMesh()->SetRelativeLocation(FVector(0, 0, -88.0f));
	GetMesh()->SetRelativeRotation(FRotator(0, -90.0f, 0));

	CurrentState = EBossState::Idle;
	CurrentBlinkAttackState = EBossBlinkAttackState::Idle;

	PawnSensing = CreateDefaultSubobject<UPawnSensingComponent>(TEXT("PawnSensing"));
	PawnSensing->bHearNoises = false;
	PawnSensing->bSeePawns = true;
	PawnSensing->SetPeripheralVisionAngle(30.0f);
	PawnSensing->SightRadius = 2000.0f;
	PawnSensing->SensingInterval = 0.01f;

	
	static ConstructorHelpers::FObjectFinder<UBehaviorTree>Boss_BT(TEXT("BehaviorTree'/Game/Blueprints/Monster/Boss/AI/BT_Boss.BT_Boss'"));

	if (Boss_BT.Succeeded())
	{
		BehaviorTree = Boss_BT.Object;
	}

	static ConstructorHelpers::FObjectFinder<UClass>ABP_Boss(TEXT("AnimBlueprint'/Game/Blueprints/Monster/Boss/Blueprints/ABP_Boss.ABP_Boss_C'"));

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

	static ConstructorHelpers::FObjectFinder<UMaterialInterface> M_Opacity(TEXT("Material'/Game/Assets/CharacterEquipment/Monster/MiniBoss/Effect/BackDash/Materials/M_Opacity.M_Opacity'"));
	if (M_Opacity.Succeeded())
	{
		OpacityMaterials = M_Opacity.Object;
	}

	static ConstructorHelpers::FObjectFinder<UMaterialInterface> M_DefaultMaterials(TEXT("Material'/Game/Assets/CharacterEquipment/Monster/Boss/Mesh/Textures/M_Boss.M_Boss'"));
	if (M_DefaultMaterials.Succeeded())
	{
		DefaultMaterials = M_DefaultMaterials.Object;
	}

	Target = nullptr;
	TargetCamera = nullptr;
}

// Called when the game starts or when spawned
void ABoss::BeginPlay()
{
	Super::BeginPlay();
	
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
}

// Called every frame
void ABoss::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	ABossAIController* AI = Cast<ABossAIController>(GetController());

	if (AI)
	{
		AI->BBComponent->SetValueAsEnum("CurrentState", (uint8)CurrentState);
		AI->BBComponent->SetValueAsEnum("CurrentBlinkAttackState", (uint8)CurrentBlinkAttackState);
		AI->BBComponent->SetValueAsEnum("CurrentBattleState", (uint8)CurrentBattleState);
	}
}

// Called to bind functionality to input
void ABoss::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

void ABoss::OnSeeCharacter(APawn * Pawn)
{
	if (Pawn->ActorHasTag("Character"))
	{
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
					CurrentState = EBossState::Battle;
				}
			}
		}
	}
}

