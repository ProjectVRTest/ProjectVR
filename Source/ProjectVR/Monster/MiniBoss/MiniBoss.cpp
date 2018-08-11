// Fill out your copyright notice in the Description page of Project Settings.

#include "MiniBoss.h"
#include "Components/SkeletalMeshComponent.h"
#include "UObject/ConstructorHelpers.h" 

#include "Perception/PawnSensingComponent.h" 
#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BlackboardComponent.h"

#include "MiniBossAIController.h"

#include "GameFramework/CharacterMovementComponent.h"

#include "Animation/AnimBlueprint.h"
#include "Weapon/MiniBossWeapon.h"
// Sets default values
AMiniBoss::AMiniBoss()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	static ConstructorHelpers::FObjectFinder<USkeletalMesh>MiniBoss_SK_Mesh(TEXT("SkeletalMesh'/Game/Assets/Monster/MiniBoss/Mesh/MB_SK_Mesh.MB_SK_Mesh'"));

	if (MiniBoss_SK_Mesh.Succeeded())
	{
		GetMesh()->SetSkeletalMesh(MiniBoss_SK_Mesh.Object);
	}

	GetMesh()->SetRelativeLocation(FVector(0, 0, -88.0f));
	GetMesh()->SetRelativeRotation(FRotator(0, -90.0f, 0));

	CurrentState = EMiniBossState::Idle;
	CurrentAnimState = EMiniBossAnimState::Wait;
	CurrentJumpState = EMiniBossJumpState::Idle;
	CurrentAttackState = EMiniBossAttackState::Idle;

	PawnSensing = CreateDefaultSubobject<UPawnSensingComponent>(TEXT("PawnSensing"));
	PawnSensing->bHearNoises = false;
	PawnSensing->bSeePawns = true;
	PawnSensing->SetPeripheralVisionAngle(30.0f);
	PawnSensing->SightRadius = 3000.0f;
	PawnSensing->SensingInterval = 0.1f;

	static ConstructorHelpers::FObjectFinder<UBehaviorTree>MiniBoss_BT(TEXT("BehaviorTree'/Game/Blueprints/Monster/MiniBoss/AI/BT_MiniBoss.BT_MiniBoss'"));

	if (MiniBoss_BT.Succeeded())
	{
		BehaviorTree = MiniBoss_BT.Object;
	}

	Target = nullptr;

	static ConstructorHelpers::FObjectFinder<UAnimMontage>Reaction_Montage(TEXT("AnimMontage'/Game/Blueprints/Monster/MiniBoss/Animation/Mutant/ReactionMontage.ReactionMontage'"));

	if (Reaction_Montage.Succeeded())
	{
		ReactionMontage = Reaction_Montage.Object;
	}

	CurrentFalling = false;
	JumpEndFlag = false;
	JumpRunCheckFlag = true;
	ParryingFlag = false;
	IsParrying = false;

	MaxHP = 100;
	CurrentHP = MaxHP;

	AIControllerClass = AMiniBossAIController::StaticClass();
	
	static ConstructorHelpers::FObjectFinder<UAnimBlueprint>ABP_MiniBos(TEXT("AnimBlueprint'/Game/Blueprints/Monster/MiniBoss/Blueprints/ABP_MiniBoss.ABP_MiniBoss'"));

	if (ABP_MiniBos.Succeeded())
	{
		GetMesh()->SetAnimationMode(EAnimationMode::AnimationBlueprint);
		GetMesh()->SetAnimInstanceClass(ABP_MiniBos.Object->GeneratedClass);
	}
	GetCharacterMovement()->MaxAcceleration = 200.0f;
	Tags.Add(TEXT("Monster"));
	Tags.Add(FName(TEXT("DisregardForLeftHand")));
	Tags.Add(FName(TEXT("DisregardForRightHand")));
}

// Called when the game starts or when spawned
void AMiniBoss::BeginPlay()
{
	Super::BeginPlay();
	
	if (PawnSensing)
	{
		PawnSensing->OnSeePawn.AddDynamic(this, &AMiniBoss::OnSeeCharacter);
	}
}

// Called every frame
void AMiniBoss::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	//GLog->Log(FString::Printf(TEXT("Velocity : %0.1f"), GetCharacterMovement()->Velocity.Size()));
	//UE_LOG(LogClass, Warning, TEXT("\n Monster pitch : %f \nYaw : %f \n Roll : %f\n "), GetActorRotation().Pitch, GetActorRotation().Yaw, GetActorRotation().Roll);
	AMiniBossAIController* AI = Cast<AMiniBossAIController>(GetController());
	if (AI)
	{
		AI->BBComponent->SetValueAsEnum("CurrentState", (uint8)CurrentState);
		AI->BBComponent->SetValueAsEnum("CurrentAnimState", (uint8)CurrentAnimState);
		AI->BBComponent->SetValueAsEnum("CurrentJumpState", (uint8)CurrentJumpState);
		AI->BBComponent->SetValueAsEnum("CurrentAttackState", (uint8)CurrentAttackState);
		CurrentFalling = GetCharacterMovement()->IsFalling();
		AI->BBComponent->SetValueAsBool("CurrentFalling", CurrentFalling);		
	}
}

// Called to bind functionality to input
void AMiniBoss::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

void AMiniBoss::OnSeeCharacter(APawn * Pawn)
{
	if (Pawn->ActorHasTag("Character"))
	{
		AMiniBossAIController* AI = Cast<AMiniBossAIController>(GetController());

		if (AI)
		{
			switch (CurrentState)
			{
			case EMiniBossState::Idle:
				AI->BBComponent->SetValueAsObject("Player", Pawn);
				Target = Pawn;
				CurrentState = EMiniBossState::Chase;
				CurrentAnimState = EMiniBossAnimState::Walk;
				break;
			case EMiniBossState::Chase:
				break;
			case EMiniBossState::Battle:
				break;
			case EMiniBossState::Patrol:
				break;
			case EMiniBossState::Dead:
				break;
			}
		}
	}
}

float AMiniBoss::TakeDamage(float Damage, FDamageEvent const & DamageEvent, AController * EventInstigator, AActor * DamageCauser)
{
	Super::TakeDamage(Damage, DamageEvent, EventInstigator, DamageCauser);

	CurrentHP -= Damage;

	UE_LOG(LogClass, Warning, TEXT("%s"), *EventInstigator->GetPawn()->GetName());

	if (CurrentHP < 0)
	{
		CurrentHP = 0;
		CurrentState = EMiniBossState::Dead;
	}

	if (ParryingFlag)
	{
		IsParrying = true;
		CurrentAnimState = EMiniBossAnimState::Parrying;
	}
	else
	{
		PlayAnimMontage(ReactionMontage);
	}

	return Damage;
}

