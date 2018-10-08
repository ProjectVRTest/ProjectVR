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
#include "Particles/ParticleSystem.h"
#include "Kismet/GameplayStatics.h"

#include "Kismet/KismetSystemLibrary.h"
#include "MyCharacter/CameraLocation.h"
#include "MyCharacter/MotionControllerCharacter.h"
#include "Components/CapsuleComponent.h"

// Sets default values
AMiniBoss::AMiniBoss()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	static ConstructorHelpers::FObjectFinder<USkeletalMesh>MiniBoss_SK_Mesh(TEXT("SkeletalMesh'/Game/Assets/CharacterEquipment/Monster/MiniBoss/Mesh/MB_SK_Mesh.MB_SK_Mesh'"));

	if (MiniBoss_SK_Mesh.Succeeded())
	{
		GetMesh()->SetSkeletalMesh(MiniBoss_SK_Mesh.Object);
	}

	GetMesh()->SetRelativeLocation(FVector(0, 0, -88.0f));
	GetMesh()->SetRelativeRotation(FRotator(0, -90.0f, 0));

	CurrentState = EMiniBossState::Idle;
	CurrentAnimState = EMiniBossAnimState::Wait;
	CurrentJumpState = EMiniBossJumpState::Idle;
	CurrentShortAttackState = EMiniBossShortAttackState::Idle;
	CurrentDashState = EMiniBossDashState::Idle;

	PawnSensing = CreateDefaultSubobject<UPawnSensingComponent>(TEXT("PawnSensing"));
	PawnSensing->bHearNoises = false;
	PawnSensing->bSeePawns = true;
	PawnSensing->SetPeripheralVisionAngle(30.0f);
	PawnSensing->SightRadius = 2000.0f;
	PawnSensing->SensingInterval = 0.01f;

	static ConstructorHelpers::FObjectFinder<UBehaviorTree>MiniBoss_BT(TEXT("BehaviorTree'/Game/Blueprints/Monster/MiniBoss/AI/BT_MiniBoss.BT_MiniBoss'"));

	if (MiniBoss_BT.Succeeded())
	{
		BehaviorTree = MiniBoss_BT.Object;
	}

	TargetCamera = nullptr;
	Target = nullptr;

	static ConstructorHelpers::FObjectFinder<UAnimMontage>AttackReverse_Montage(TEXT("AnimMontage'/Game/Blueprints/Monster/MiniBoss/Animation/AM_AttackReverse.AM_AttackReverse'"));

	if (AttackReverse_Montage.Succeeded())
	{
		AttackReverseMontage= AttackReverse_Montage.Object;
	}

	CurrentFalling = false;
	ParryingFlag = false;
	AttackCompleteFlag = false;
	IsParrying = false;
	IsAttack = false; //공격할수 있게 해줌
	StabFlag = false;
	TwoHandWidthFlag = false;
	MaxHP = 100000.0f;
	CurrentHP = MaxHP;

	static ConstructorHelpers::FObjectFinder<UMaterialInterface> M_Opacity(TEXT("Material'/Game/Assets/CharacterEquipment/Monster/MiniBoss/Effect/BackDash/Materials/M_Opacity.M_Opacity'"));
	if (M_Opacity.Succeeded())
	{
		OpacityMaterials = M_Opacity.Object;
	}

	static ConstructorHelpers::FObjectFinder<UMaterialInterface> M_DefaultMaterials(TEXT("Material'/Game/Assets/CharacterEquipment/Monster/MiniBoss/Materials/M_MiniBoss.M_MiniBoss'"));
	if (M_DefaultMaterials.Succeeded())
	{
		DefaultMaterials = M_DefaultMaterials.Object;
	}

	AIControllerClass = AMiniBossAIController::StaticClass();
	
	static ConstructorHelpers::FObjectFinder<UParticleSystem>PT_AfterImageStartEffect(TEXT("ParticleSystem'/Game/Assets/CharacterEquipment/Monster/MiniBoss/Effect/BackDash/StartEffect.StartEffect'"));
	if (PT_AfterImageStartEffect.Succeeded())
	{
		AfterImageStartEffect = PT_AfterImageStartEffect.Object;
	}

	static ConstructorHelpers::FObjectFinder<UParticleSystem>PT_AfterImageEndEffect(TEXT("ParticleSystem'/Game/Assets/CharacterEquipment/Monster/MiniBoss/Effect/BackDash/EndEffect.EndEffect'"));
	if (PT_AfterImageEndEffect.Succeeded())
	{
		AfterImageEndEffect = PT_AfterImageEndEffect.Object;
	}
	SwordWaveCount = 1;
	ParryingPointCount = 0;

	SwordWaveSpawn = CreateDefaultSubobject<USceneComponent>(TEXT("SwordWaveSpawn"));
	SwordWaveSpawn->SetupAttachment(GetRootComponent());
	SwordWaveSpawn->SetRelativeLocation(FVector(80.0f, 0, 50.0f));

	static ConstructorHelpers::FObjectFinder<UClass>ABP_MiniBos(TEXT("AnimBlueprint'/Game/Blueprints/Monster/MiniBoss/Blueprints/ABP_MiniBoss.ABP_MiniBoss_C'"));

	if (ABP_MiniBos.Succeeded())
	{
		UClass* MiniBossAnimBlueprint = ABP_MiniBos.Object;

		if (MiniBossAnimBlueprint)
		{
			GetMesh()->SetAnimationMode(EAnimationMode::AnimationBlueprint);
			GetMesh()->SetAnimInstanceClass(MiniBossAnimBlueprint);
		}		
	}

	GetCharacterMovement()->MaxAcceleration = 2048.0f;
	Tags.Add(TEXT("Monster"));
	Tags.Add(FName(TEXT("DisregardForLeftHand")));
	Tags.Add(FName(TEXT("DisregardForRightHand")));
}

// Called when the game starts or when spawned
void AMiniBoss::BeginPlay()
{
	Super::BeginPlay();
	
	
	FActorSpawnParameters SpawnActorOption;
	SpawnActorOption.Owner = this;
	SpawnActorOption.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

	FAttachmentTransformRules AttachRules(EAttachmentRule::SnapToTarget, EAttachmentRule::SnapToTarget, EAttachmentRule::KeepWorld, false);

	Sword = GetWorld()->SpawnActor<AMiniBossWeapon>(Sword->StaticClass(), SpawnActorOption);

	if (Sword)
	{
		Sword->AttachToComponent(GetMesh(), AttachRules, FName(TEXT("weapon001소켓")));
	}
	
	if (PawnSensing)
	{
		PawnSensing->OnSeePawn.AddDynamic(this, &AMiniBoss::OnSeeCharacter);
	}
}

// Called every frame
void AMiniBoss::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
	AMiniBossAIController* AI = Cast<AMiniBossAIController>(GetController());
	FVector Origin;
	FVector BoxExtent;
	float SphereRadius;
	UKismetSystemLibrary::GetComponentBounds(GetMesh(), Origin, BoxExtent, SphereRadius);
	
	//GLog->Log(FString::Printf(TEXT("%d"), AttackCompleteFlag));

	/*GLog->Log(FString::Printf(TEXT("Origin X : %0.1f  Y : %0.1f Z : %0.1f"), Origin.X, Origin.Y, Origin.Z));
	GLog->Log(FString::Printf(TEXT("Origin %0.1f"), Origin.Size()));*/
	//GLog->Log(FString::Printf(TEXT("Origin %d : \n BoxExtent : %d \n SphereRadius : %0.1f"),Origin.Size(),BoxExtent.Size(),SphereRadius));

	//GLog->Log(FString::Printf(TEXT("HP : %f"),CurrentHP));

	//GLog->Log(FString::Printf(TEXT("%f"), GetCharacterMovement()->Velocity.Size()));

	if (AI)
	{
		AI->BBComponent->SetValueAsEnum("CurrentState", (uint8)CurrentState);
		AI->BBComponent->SetValueAsEnum("CurrentAnimState", (uint8)CurrentAnimState);
		AI->BBComponent->SetValueAsEnum("CurrentJumpState", (uint8)CurrentJumpState);
		AI->BBComponent->SetValueAsEnum("CurrentAttackState", (uint8)CurrentAttackState);
		AI->BBComponent->SetValueAsEnum("CurrentShortAttackState", (uint8)CurrentShortAttackState);
		AI->BBComponent->SetValueAsEnum("CurrentWaveAttackState", (uint8)CurrentWaveAttackState);
		AI->BBComponent->SetValueAsEnum("CurrentComboAttackState", (uint8)CurrentComboAttackState);
		AI->BBComponent->SetValueAsEnum("CurrentBackAttackState", (uint8)CurrentBackAttackState);
		AI->BBComponent->SetValueAsEnum("CurrentDashState", (uint8)CurrentDashState);
		AI->BBComponent->SetValueAsEnum("CurrentParryingState", (uint8)CurrentParryingState);

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
				if (!Target)
				{
					AMotionControllerCharacter* MyCharacter = Cast<AMotionControllerCharacter>(Pawn);

					if (MyCharacter)
					{
						TargetCamera = MyCharacter->CameraLocation;
						AI->BBComponent->SetValueAsObject("PlayerCamera", TargetCamera);
					}
					AI->BBComponent->SetValueAsObject("Player", Pawn);
					Target = Pawn;
					CurrentState = EMiniBossState::Chase;
					CurrentAnimState = EMiniBossAnimState::Walk;
				}				
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

	//UE_LOG(LogClass, Warning, TEXT("%s"), *EventInstigator->GetPawn()->GetName());

	if (CurrentHP < 0)
	{
		CurrentHP = 0;
		CurrentState = EMiniBossState::Dead;
	}
	
	if (ParryingFlag)
	{
		IsParrying = true;
		//CurrentAnimState = EMiniBossAnimState::ParryingReady;
	}
	else
	{
		//PlayAnimMontage(ReactionMontage);
	}

	return Damage;
}
