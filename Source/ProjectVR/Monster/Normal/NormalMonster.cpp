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

	static ConstructorHelpers::FObjectFinder<USkeletalMesh>Normal_Monster_SK_Mesh(TEXT("SkeletalMesh'/Game/Assets/Monster/NormalMonster/Mesh/Character/SK_NormalMonster.SK_NormalMonster'"));

	if (Normal_Monster_SK_Mesh.Succeeded())
	{
		GetMesh()->SetSkeletalMesh(Normal_Monster_SK_Mesh.Object);
	}

	GetMesh()->SetRelativeLocation(FVector(0, 0, -88.0f));
	GetMesh()->SetRelativeRotation(FRotator(0, -90.0f, 0));

	CurrentState = ENormalMonsterState::Idle;
	CurrentAnimState = ENormalMonsterAnimState::Wait;
	CurrentIdleState = ENormalMonsterIdleState::Wait;
	CurrentAttackState = ENormalMonsterAttackState::Idle;

	PawnSensing = CreateDefaultSubobject<UPawnSensingComponent>(TEXT("PawnSensing"));
	PawnSensing->bHearNoises = false;
	PawnSensing->bSeePawns = true;
	PawnSensing->SetPeripheralVisionAngle(30.0f);
	PawnSensing->SightRadius = 3000.0f;
	PawnSensing->SensingInterval = 0.1f;

	static ConstructorHelpers::FObjectFinder<UBehaviorTree>NormalSwordMonster_BT(TEXT("BehaviorTree'/Game/Blueprints/Monster/Normal/AI/BT_NormalSwordMonster.BT_NormalSwordMonster'"));

	if (NormalSwordMonster_BT.Succeeded())
	{
		SwordBehaviorTree = NormalSwordMonster_BT.Object;
	}

	static ConstructorHelpers::FObjectFinder<UBehaviorTree>NormalArcherMonster_BT(TEXT("BehaviorTree'/Game/Blueprints/Monster/Normal/AI/BT_NormalArcherMonster.BT_NormalArcherMonster'"));

	if (NormalArcherMonster_BT.Succeeded())
	{
		ArcherBehaviorTree = NormalArcherMonster_BT.Object;
	}

	MaxHP = 100;
	CurrentHP = MaxHP;

	AIControllerClass = ANormalMonsterAIController::StaticClass();

	static ConstructorHelpers::FObjectFinder<UClass>ABP_NormalMonster(TEXT("AnimBlueprint'/Game/Blueprints/Monster/Normal/Blueprints/ABP_NormalMonster.ABP_NormalMonster_C'"));

	if (ABP_NormalMonster.Succeeded())
	{
		UClass* NormalMonsterAnimBlueprint = ABP_NormalMonster.Object;
		
		if (NormalMonsterAnimBlueprint)
		{
			GetMesh()->SetAnimationMode(EAnimationMode::AnimationBlueprint);
			GetMesh()->SetAnimInstanceClass(NormalMonsterAnimBlueprint);
		}		
	}

	Target = nullptr;
	AttackEndFlag = false;
	GetCharacterMovement()->MaxWalkSpeed = 450.0f;
	Tags.Add(TEXT("Monster"));
	Tags.Add(FName(TEXT("DisregardForLeftHand")));
	Tags.Add(FName(TEXT("DisregardForRightHand")));
}

// Called when the game starts or when spawned
void ANormalMonster::BeginPlay()
{
	Super::BeginPlay();
	ANormalMonsterAIController* AI = Cast<ANormalMonsterAIController>(GetController());

	if (AI)
	{
		AI->BBComponent->SetValueAsEnum(TEXT("MonsterKind"), (uint8)MonsterKind);
	}

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
		AI->BBComponent->SetValueAsEnum("CurrentStabAttackState", (uint8)CurrentStabAttackState);
		AI->BBComponent->SetValueAsEnum("CurrentComboAttackState", (uint8)CurrentComboAttackState);
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
			if (CurrentAnimState == ENormalMonsterAnimState::AttackWait)
			{
				return;
			}
			else
			{
				if (Target == nullptr)
				{
					Target = Pawn;
					AI->BBComponent->SetValueAsObject("Player", Pawn);
					CurrentState = ENormalMonsterState::Chase;

					if (MonsterKind == ENormalMonsterKind::SwordMan)
					{
						CurrentAnimState = ENormalMonsterAnimState::Wait;
					}
					else
					{
						CurrentAnimState = ENormalMonsterAnimState::Walk;
					}
				}
			}
		}
	}
}

float ANormalMonster::TakeDamage(float Damage, FDamageEvent const & DamageEvent, AController * EventInstigator, AActor * DamageCauser)
{
	Super::TakeDamage(Damage, DamageEvent, EventInstigator, DamageCauser);

	if (CurrentAnimState == ENormalMonsterAnimState::AttackWait)
	{
		ANormalMonsterAIController* AI = Cast<ANormalMonsterAIController>(GetController());

		if (AI)
		{
			if (CurrentAnimState == ENormalMonsterAnimState::AttackWait)
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
					case ENormalMonsterKind::Archer:
						CurrentAnimState = ENormalMonsterAnimState::Walk;
						break;
					}					
				}		

				CurrentHP -= Damage;
			}
			else
			{
				CurrentHP -= Damage;
			}			
		}
	}
	return Damage;
}

