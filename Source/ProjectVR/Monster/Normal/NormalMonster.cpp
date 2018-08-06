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

// Sets default values
ANormalMonster::ANormalMonster()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	static ConstructorHelpers::FObjectFinder<USkeletalMesh>Normal_Monster_SK_Mesh(TEXT("SkeletalMesh'/Game/Assets/Monster/NormalMonster/Mesh/Normal_Monster.Normal_Monster'"));

	if (Normal_Monster_SK_Mesh.Succeeded())
	{
		GetMesh()->SetSkeletalMesh(Normal_Monster_SK_Mesh.Object);
	}

	GetMesh()->SetRelativeLocation(FVector(0, 0, -88.0f));
	GetMesh()->SetRelativeRotation(FRotator(0, -90.0f, 0));

	CurrentState = ENormalMonsterState::Idle;
	CurrentAnimState = ENormalMonsterAnimState::Wait;
	CurrentIdleState = ENormalMonsterIdleState::Wait;
	CurrentAttackState = ENormalMonsterAttackState::Stand;

	PawnSensing = CreateDefaultSubobject<UPawnSensingComponent>(TEXT("PawnSensing"));
	PawnSensing->bHearNoises = false;
	PawnSensing->bSeePawns = true;
	PawnSensing->SetPeripheralVisionAngle(30.0f);
	PawnSensing->SightRadius = 3000.0f;
	PawnSensing->SensingInterval = 0.01f;

	static ConstructorHelpers::FObjectFinder<UBehaviorTree>NormalMonster_BT(TEXT("BehaviorTree'/Game/Blueprints/Monster/Normal/AI/BT_NormalMonster.BT_NormalMonster'"));

	if (NormalMonster_BT.Succeeded())
	{
		BehaviorTree = NormalMonster_BT.Object;
	}

	MaxHP = 100;
	CurrentHP = MaxHP;

	AIControllerClass = ANormalMonsterAIController::StaticClass();

	static ConstructorHelpers::FObjectFinder<UAnimBlueprint>ABP_NormalMonster(TEXT("AnimBlueprint'/Game/Blueprints/Monster/Normal/Blueprints/ABP_NormalMonster.ABP_NormalMonster'"));

	if (ABP_NormalMonster.Succeeded())
	{
		GetMesh()->SetAnimationMode(EAnimationMode::AnimationBlueprint);
		GetMesh()->SetAnimInstanceClass(ABP_NormalMonster.Object->GeneratedClass);
	}

	GetCharacterMovement()->MaxWalkSpeed = 450.0f;
	Tags.Add(TEXT("Monster"));
}

// Called when the game starts or when spawned
void ANormalMonster::BeginPlay()
{
	Super::BeginPlay();
	
	if (PawnSensing)
	{
		PawnSensing->OnSeePawn.AddDynamic(this, &ANormalMonster::OnSeeCharacter);
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
	}
}

// Called to bind functionality to input
void ANormalMonster::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);


}

void ANormalMonster::OnSeeCharacter(APawn * Pawn)
{
	if (Pawn->ActorHasTag("Character"))
	{
		ANormalMonsterAIController* AI = Cast<ANormalMonsterAIController>(GetController());

		if (AI)
		{
			switch (CurrentState)
			{
			case ENormalMonsterState::Idle:
				AI->BBComponent->SetValueAsObject("Player", Pawn);
				CurrentState = ENormalMonsterState::Chase;
				CurrentAnimState = ENormalMonsterAnimState::Walk;
				break;
			case ENormalMonsterState::Patrol:
				AI->BBComponent->SetValueAsObject("Player", Pawn);
				CurrentState = ENormalMonsterState::Chase;
				CurrentAnimState = ENormalMonsterAnimState::Walk;
				break;
			}
		}
	}
}

float ANormalMonster::TakeDamage(float Damage, FDamageEvent const & DamageEvent, AController * EventInstigator, AActor * DamageCauser)
{
	Super::TakeDamage(Damage, DamageEvent, EventInstigator, DamageCauser);

	return Damage;
}

