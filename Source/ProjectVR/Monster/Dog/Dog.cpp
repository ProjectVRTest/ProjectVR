// Fill out your copyright notice in the Description page of Project Settings.

#include "Dog.h"

#include "UObject/ConstructorHelpers.h"
#include "Components/CapsuleComponent.h"
#include "Components/BoxComponent.h"

#include "GameFramework/CharacterMovementComponent.h"
#include "Perception/PawnSensingComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "Animation/AnimSequence.h"

#include "Kismet/GameplayStatics.h"
#include "MyCharacter/MotionControllerCharacter.h"

#include "Monster/Dog/DogAIController.h"
#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "BehaviorTree/BehaviorTreeComponent.h"

#include "Animation/AnimMontage.h"
#include "Animation/AnimBlueprint.h"

#include "Camera/CameraComponent.h"

#include "Kismet/KismetMathLibrary.h"

#include "HandMotionController/RightHandMotionController.h"
#include "HandMotionController/LeftHandMotionController.h"
#include "Components/SphereComponent.h"

#include "Equipment/PlayerSword.h"			// 물고 있을때 검의 데미지를 안 받기 위해서 작업
// Sets default values
ADog::ADog()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	static ConstructorHelpers::FObjectFinder<USkeletalMesh>MonsterMesh(TEXT("SkeletalMesh'/Game/Assets/CharacterEquipment/Monster/Dog/Mesh2/MON_DOG_MESH.MON_DOG_MESH'"));
	if (MonsterMesh.Succeeded())
	{
		GetMesh()->SetSkeletalMesh(MonsterMesh.Object);
	}
	static ConstructorHelpers::FObjectFinder<UBehaviorTree>Monster_BehaviorTree(TEXT("BehaviorTree'/Game/Blueprints/Monster/Dog/AI/RagdollDogBT_2.RagdollDogBT_2'"));
	if (Monster_BehaviorTree.Succeeded())
	{
		BehaviorTree = Monster_BehaviorTree.Object;
	}

	//기존에 사용 하던 UAnimBlueprint는 패키징 이후에 엔진이 찾을수 없는것으로 판단
	//새롭게 UClass로 오브젝트를 찾고 꽂아 넣는 형식으로 바꿈
	static ConstructorHelpers::FObjectFinder<UClass>Monster_AnimBlueprint(TEXT("AnimBlueprint'/Game/Blueprints/Monster/Dog/Blueprints2/ABP_Dog_3.ABP_Dog_3_C'"));

	if (Monster_AnimBlueprint.Succeeded())
	{
		UClass* DogAnimBlueprint = Monster_AnimBlueprint.Object;

		if (DogAnimBlueprint)
		{
			GetMesh()->SetAnimationMode(EAnimationMode::AnimationBlueprint);
			GetMesh()->SetAnimInstanceClass(DogAnimBlueprint);
		}
	}


		static ConstructorHelpers::FObjectFinder<UMaterialInterface>Monster_Material(TEXT("Material'/Game/Assets/CharacterEquipment/Monster/Dog/Materials/M_Dog.M_Dog'"));
		if (Monster_Material.Succeeded())
		{
			NomalMaterial = Monster_Material.Object;
			//GetMesh()->SetMaterial(0, Monster_Material.Object);
		}
	

		static ConstructorHelpers::FObjectFinder<UMaterialInterface>Monster_Material_Elite(TEXT("Material'/Game/Assets/CharacterEquipment/Monster/Dog/Materials/NewFolder/M_Dog_Elite.M_Dog_Elite'"));
		if (Monster_Material_Elite.Succeeded())
		{
			EliteMaterial = Monster_Material_Elite.Object;
			//GetMesh()->SetMaterial(0, Monster_Material_Elite.Object);
		}



	DogAttackCollision = CreateDefaultSubobject<UBoxComponent>(TEXT("DogAttack"));
	DogAttackCollision->SetupAttachment(GetMesh(), TEXT("HeadSocket"));

	PawnSensing = CreateDefaultSubobject<UPawnSensingComponent>(TEXT("PawnSensing"));

	AIControllerClass = ADogAIController::StaticClass();

	ADog::GetCapsuleComponent()->SetCapsuleHalfHeight(55.0f);
	ADog::GetCapsuleComponent()->SetCapsuleRadius(30.0f);
	GetMesh()->SetRelativeLocation(FVector(-20.0f, 0.0f, -55.0f));
	GetMesh()->SetRelativeRotation(FRotator(0.0f, -90.0f, 0.0f));

	GetCapsuleComponent()->SetCollisionProfileName("IgnoreOnlyPawn");

	DogAttackCollision->SetCollisionProfileName(TEXT("OverlapAll"));
	DogAttackCollision->SetRelativeLocation(FVector(10.0f, -10.0f, 0.0f));
	DogAttackCollision->SetRelativeScale3D(FVector(0.6f, 0.6f, 0.6f));
	DogAttackCollision->SetActive(false);
	DogAttackCollision->ComponentTags.Add("DogAttackCollision");
	DogAttackCollision->bGenerateOverlapEvents = false;

	PawnSensing->bHearNoises = false;
	PawnSensing->bSeePawns = true;
	PawnSensing->SetPeripheralVisionAngle(40.0f);
	PawnSensing->SightRadius = 1200.0f;
	PawnSensing->SensingInterval = 0.1f;

	bOnLand = true;			// 땅에 있는지
	Landing = false;		// 착지인지아닌지

	MaxHP = 1.0f;
	CurrentHP = MaxHP;
	bIsDeath = false;
	bIsDetach = false;

	AttackWaite = false;

	GetMesh()->SetSimulatePhysics(false);
	GetMesh()->SetCollisionProfileName("Ragdoll");
	GetCharacterMovement()->MaxWalkSpeed = 0.0f;
	Tags.Add("Monster");
	Tags.Add("Dog");
	Tags.Add(FName(TEXT("DisregardForLeftHand")));
	Tags.Add(FName(TEXT("DisregardForRightHand")));
}

// Called when the game starts or when spawned
void ADog::BeginPlay()
{
	Super::BeginPlay();

	if(bIsElite)
		GetMesh()->SetMaterial(0, EliteMaterial);
	else
		GetMesh()->SetMaterial(0, NomalMaterial);

	CurrentDogState = EDogState::Idle;
	CurrentDogAnimState = EDogAnimState::Idle;
	CurrentDogBattleState = EDogBattleState::Nothing;
	CurrentDogAirState = EDogAirState::Nothing;
	CurrentDogJumpState = EDogJumpState::Nothing;
	CurrentDogCircleState = EDogCircleState::Nothing;

	DogAttackCollision->OnComponentBeginOverlap.AddDynamic(this, &ADog::OnAttackCollisionOverlap);

	if (PawnSensing)
	{
		PawnSensing->OnSeePawn.AddDynamic(this, &ADog::OnSeePlayer);
	}
}

// Called every frame
void ADog::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	AI = Cast<ADogAIController>(GetController());
	FFindFloorResult FloorDistance;
	GetCharacterMovement()->ComputeFloorDist(GetCapsuleComponent()->GetComponentLocation(), 10000.0f, 10000.0f, FloorDistance, 34.0f);

	// 블랙보드에 매 틱마다 정보 전달
	if (AI)
	{
		AI->BBComponent->SetValueAsEnum("CurrentDogState", (uint8)CurrentDogState);
		AI->BBComponent->SetValueAsEnum("CurrentDogAnimState", (uint8)CurrentDogAnimState);
		AI->BBComponent->SetValueAsEnum("CurrentDogJumpState", (uint8)CurrentDogJumpState);
		AI->BBComponent->SetValueAsEnum("CurrentDogCircleState", (uint8)CurrentDogCircleState);
		AI->BBComponent->SetValueAsEnum("CurrentDogBattleState", (uint8)CurrentDogBattleState);
		AI->BBComponent->SetValueAsEnum("CurrentDogAirState", (uint8)CurrentDogAirState);
		AI->BBComponent->SetValueAsObject("AttachActor", AttachActor);
		AI->BBComponent->SetValueAsBool("bOnLand", bOnLand);
		AI->BBComponent->SetValueAsBool("DeathFlag", bIsDeath);
		AI->BBComponent->SetValueAsFloat("HP", CurrentHP);
		height = FloorDistance.FloorDist;
		CurrentFalling = GetCharacterMovement()->IsFalling();
	}

}

// Called to bind functionality to input
void ADog::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

void ADog::OnSeePlayer(APawn * Pawn)
{
	FFindFloorResult FloorDistance;;
	GetCharacterMovement()->ComputeFloorDist(GetCapsuleComponent()->GetComponentLocation(), 10000.0f, 10000.0f, FloorDistance, 34, 0);

	if (Pawn->ActorHasTag("Character") && FloorDistance.FloorDist < 3.0f)
	{
		ADogAIController* AI = Cast<ADogAIController>(GetController());

		if (AI && !AI->BBComponent->GetValueAsObject("Player"))
		{
			Target = Pawn;
			CurrentDogState = EDogState::Chase;
			CurrentDogAnimState = EDogAnimState::Run;
			CurrentDogJumpState = EDogJumpState::Nothing;
			CurrentDogCircleState = EDogCircleState::Nothing;
			AI->BBComponent->SetValueAsObject("Player", Pawn);
			GetCharacterMovement()->MaxWalkSpeed = 550.0f;
		}
	}
}


void ADog::OnAttackCollisionOverlap(UPrimitiveComponent * OverlappedComp, AActor * OtherActor, UPrimitiveComponent * OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult)
{
	if (OtherComp->ComponentHasTag("Head"))
	{
		UGameplayStatics::ApplyDamage(OtherActor, 20, UGameplayStatics::GetPlayerController(GetWorld(), 0), this, nullptr);		// 오버랩된 액터에 데미지 전달

		AMotionControllerCharacter* Character = Cast<AMotionControllerCharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));

		if (Character)
		{
			ARightHandMotionController* RightController = Cast<ARightHandMotionController>(Character->RightHand);

			if (!RightController->AttachDog)
			{
				RightController->AttachDog = this;

				DogAttackCollision->bGenerateOverlapEvents = false;			// 입 콜리전 비활성화

				// KeepRelative : 손의 각도가 같으면 붙는 각도도 일정함
				FAttachmentTransformRules AttachRules(EAttachmentRule::SnapToTarget, EAttachmentRule::SnapToTarget, EAttachmentRule::KeepWorld, true);

				AttachToComponent(RightController->AttachDogPosition, AttachRules);

				GetMesh()->SetAllBodiesBelowSimulatePhysics("Bip002-Spine1", true, true);			// Neck이하는 모조리 피직스 부여

				SetActorRelativeLocation(FVector(0.0f, 0.0f, 0.0f));
				SetActorRelativeRotation(FRotator(0.0f, 0.0f, 0.0f));

				AttachActor = RightController;
				AI->BBComponent->SetValueAsBool("bIsBiting", true);
			}
		}
	}
}

float ADog::TakeDamage(float Damage, FDamageEvent const & DamageEvent, AController * EventInstigator, AActor * DamageCauser)
{
	APlayerSword* Sword = Cast<APlayerSword>(DamageCauser);
	if (CurrentDogState == EDogState::Bite && Sword)
	{
		return 0;
	}
	CurrentHP -= Damage;

	if (CurrentHP < 0.0f)
	{
		DogAttackCollision->bGenerateOverlapEvents = false;

		if (CurrentDogState == EDogState::Bite)
		{
			CurrentDogAnimState = EDogAnimState::FallingDeath;
			bIsDeath = true;
			bpunchDetach = true;
		}
		else if (CurrentDogState == EDogState::Battle)
		{
			CurrentDogState = EDogState::Death;
		}
	}

	return Damage;
}

