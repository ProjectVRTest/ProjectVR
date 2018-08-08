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
// Sets default values
ADog::ADog()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	static ConstructorHelpers::FObjectFinder<USkeletalMesh>MonsterMesh(TEXT("SkeletalMesh'/Game/Assets/Monster/Dog/Mesh/MON_DOG_MESH.MON_DOG_MESH'"));
	if (MonsterMesh.Succeeded())
	{
		GetMesh()->SetSkeletalMesh(MonsterMesh.Object);
	}
	static ConstructorHelpers::FObjectFinder<UBehaviorTree>Monster_BehaviorTree(TEXT("BehaviorTree'/Game/Blueprints/Monster/Dog/AI/RagdollDogBT.RagdollDogBT'"));
	if (Monster_BehaviorTree.Succeeded())
	{
		BehaviorTree = Monster_BehaviorTree.Object;
	}

	static ConstructorHelpers::FObjectFinder<UAnimBlueprint>Monster_AnimBlueprint(TEXT("AnimBlueprint'/Game/Blueprints/Monster/Dog/Blueprints/ABP_Dog.ABP_Dog'"));

	if (Monster_AnimBlueprint.Succeeded())
	{
		GetMesh()->SetAnimationMode(EAnimationMode::AnimationBlueprint);
		GetMesh()->SetAnimInstanceClass(Monster_AnimBlueprint.Object->GeneratedClass);
	}

	static ConstructorHelpers::FObjectFinder<UMaterialInterface>Monster_Material(TEXT("Material'/Game/Assets/Monster/Dog/Materials/M_Dog.M_Dog'"));
	if (Monster_Material.Succeeded())
	{
		GetMesh()->SetMaterial(0, Monster_Material.Object);
	}

	DogAttackCollision = CreateDefaultSubobject<UBoxComponent>(TEXT("DogAttack"));
	DogAttackCollision->SetupAttachment(GetMesh(), TEXT("HeadSocket"));

	PawnSensing = CreateDefaultSubobject<UPawnSensingComponent>(TEXT("PawnSensing"));

	AIControllerClass = ADogAIController::StaticClass();

	ADog::GetCapsuleComponent()->SetCapsuleHalfHeight(55.0f);
	ADog::GetCapsuleComponent()->SetCapsuleRadius(30.0f);
	GetMesh()->SetRelativeLocation(FVector(-20.0f, 0.0f, -50.0f));
	GetMesh()->SetRelativeRotation(FRotator(0.0f, -90.0f, 0.0f));

	DogAttackCollision->SetCollisionProfileName(TEXT("OverlapAll"));
	DogAttackCollision->SetRelativeLocation(FVector(0.0f, 72.0f, 82.0f));
	DogAttackCollision->SetRelativeScale3D(FVector(0.8f, 0.8f, 0.8f));
	DogAttackCollision->SetActive(false);
	DogAttackCollision->ComponentTags.Add("DogAttackCollision");


	PawnSensing->bHearNoises = false;
	PawnSensing->bSeePawns = true;
	PawnSensing->SetPeripheralVisionAngle(40.0f);
	PawnSensing->SightRadius = 1200.0f;
	PawnSensing->SensingInterval = 0.1f;

	bIsAttack = false;
	OnLandFlag = false;

	MaxHP = 1;
	CurrentHP = MaxHP;
	bIsDeath = false;

	GetMesh()->SetSimulatePhysics(false);
	GetMesh()->SetCollisionProfileName("Ragdoll");
	GetCharacterMovement()->MaxWalkSpeed = 0.0f;
	Tags.Add("Monster");
	Tags.Add(FName(TEXT("DisregardForLeftHand")));
	Tags.Add(FName(TEXT("DisregardForRightHand")));
}

// Called when the game starts or when spawned
void ADog::BeginPlay()
{
	Super::BeginPlay();
	
	CurrentDogState = EDogState::Idle;
	CurrentDogAnimState = EDogAnimState::Idle;
	CurrentDogJumpState = EDogJumpState::Nothing;

	if (PawnSensing)
	{
		PawnSensing->OnSeePawn.AddDynamic(this, &ADog::OnSeePlayer);
	}

	GetMesh()->OnComponentBeginOverlap.AddDynamic(this, &ADog::OnBodyOverlap);
	DogAttackCollision->OnComponentBeginOverlap.AddDynamic(this, &ADog::OnHeadOverlap);		// ������ �̺�Ʈ�� �߻���ų �� �ֵ��� ����
}

// Called every frame
void ADog::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	GetCapsuleComponent()->SetRelativeRotation(FRotator(0.0f, GetCapsuleComponent()->GetComponentRotation().Yaw, 0.0f));

	FFindFloorResult FloorDistance;;
	GetCharacterMovement()->ComputeFloorDist(GetCapsuleComponent()->GetComponentLocation(), 10000.0f, 10000.0f, FloorDistance, 34, 0);

	if (OnLandFlag)
	{
		AI = Cast<ADogAIController>(GetController());

		if (AI->BBComponent->GetValueAsFloat("DistanceWithLand") < 3.0f)
		{
			if (CurrentHP > 0.0f)
			{
				// ���� ������ ����� -> ���󰡴� �׼��� �߰��Ǹ� #1, #2�� ���ܵΰ� �������� �½�ũ �߰��ؼ� ������ ��
				GetMesh()->SetAllBodiesBelowSimulatePhysics("Bip002-Spine", false, true);
				GetMesh()->SetAllBodiesBelowSimulatePhysics("Bip002-Neck", false, true);
				GetMesh()->SetAllBodiesBelowSimulatePhysics("Bip002-R-Thigh", false, true);
				GetMesh()->SetAllBodiesBelowSimulatePhysics("Bip002-L-Thigh", false, true);
				GetMesh()->SetAllBodiesBelowSimulatePhysics("Bip002-Tail", false, true);
				GetCapsuleComponent()->SetSimulatePhysics(false);		// #1
				OnLandFlag = false;		// #2
			}
			else
			{
				CurrentDogState = EDogState::Death;
				CurrentDogAnimState = EDogAnimState::Nothing;
				CurrentDogJumpState = EDogJumpState::Nothing;
			}
		}
	}

	if (AI)
	{
		AI->BBComponent->SetValueAsEnum("CurrentDogState", (uint8)CurrentDogState);
		AI->BBComponent->SetValueAsEnum("CurrentDogAnimState", (uint8)CurrentDogAnimState);
		AI->BBComponent->SetValueAsEnum("CurrentDogJumpState", (uint8)CurrentDogJumpState);
		AI->BBComponent->SetValueAsFloat("DistanceWithLand", FloorDistance.FloorDist);

		CurrentFalling = GetCharacterMovement()->IsFalling();
	}

	if (AttachActor)
	{
		// ��ġ ���� ����
		SetActorRelativeRotation(FRotator(0.0f, 0.0f, 0.0f));		
		SetActorRelativeLocation(FVector(0.0f, 0.0f, 0.0f));

		FVector ForceVector = GetMesh()->GetPhysicsAngularVelocity();
		AMotionControllerCharacter* Character = Cast<AMotionControllerCharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));

		// ����Ʈ ������ ���� Ƚ�� �����Ǹ� ���� ������ 8�� ������ - ������ Ʋ�� �������°� ����
		if (GetMesh()->GetPhysicsLinearVelocity().Size() >= 300.0f && GetMesh()->GetPhysicsAngularVelocity().Size() >= 400.0f)
		{
			point++;
		}
		else
		{
			if(prelinear < 300 && preangular < 400)		// �� �ӵ��� �ּ��Ѱ� - GetPhysicsVelocity�� ������ �̵��ϸ� ���� �۾��� -> ���� �񱳸��ؼ� ������ ������ ����Ʈ ����
				point--;
		}

		prelinear = GetMesh()->GetPhysicsLinearVelocity().Size();
		preangular = GetMesh()->GetPhysicsAngularVelocity().Size();

		if (point >= 8 || bpunchDetach)
		{
			CurrentDogState = EDogState::Hurled;
			CurrentDogAnimState = EDogAnimState::Fly;
			CurrentDogJumpState = EDogJumpState::Nothing;

			point = 0;
			DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);		// ��
			AttachActor = nullptr;		// ���� ���� ���� �ʱ�ȭ
			bIsAttack = false;			// ���ݻ��� �ƴ�/
			bpunchDetach = false;
			// ĳ������ �������� �پ��ִ� ���͸� �ʱ�ȭ
			AMotionControllerCharacter* Character = Cast<AMotionControllerCharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));
			Character->RightHand->AttachDog = nullptr;

			// ���󰡴� ����
			FVector Direction = Character->Camera->GetUpVector() + Character->Camera->GetForwardVector();
			
			// ���󰡴� ���� ����
			GetCapsuleComponent()->SetPhysicsLinearVelocity(Direction* 500.0f);
			GetCapsuleComponent()->SetPhysicsAngularVelocity(Direction* 500.0f);
			GetCapsuleComponent()->SetSimulatePhysics(true);
			GetCapsuleComponent()->AddForce(Direction * 500.0f);
			
			OnLandFlag = true;		// �ٴڿ� ����� �� �ѹ��� ����
		}
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

			AI->BBComponent->SetValueAsObject("Player", Pawn);
			GetCharacterMovement()->MaxWalkSpeed = 550.0f;

		}
	}
}

void ADog::AttachVirtualHandWithHead()
{
}

void ADog::OnBodyOverlap(UPrimitiveComponent * OverlappedComp, AActor * OtherActor, UPrimitiveComponent * OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult)
{
	
}

void ADog::OnHeadOverlap(UPrimitiveComponent * OverlappedComp, AActor * OtherActor, UPrimitiveComponent * OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult)
{
	//if (OtherActor->ActorHasTag("LeftHand"))
	//{
	//	ALeftHandMotionController* LeftHand = Cast<ALeftHandMotionController>(OtherActor);

	//	if (LeftHand)
	//	{
	//		// �޼��� �ӵ��� �ּҼӵ� �̻��� �� �������� ��
	//		if (LeftHand->GrabSphere->GetPhysicsLinearVelocity().Size() >= 350.0f)
	//		{
	//			bpunchDetach = true;
	//		}
	//	}
	//}
}

float ADog::TakeDamage(float Damage, FDamageEvent const & DamageEvent, AController * EventInstigator, AActor * DamageCauser)
{
	CurrentHP -= Damage;

	if (CurrentHP < 0.0f)
	{
		bIsDeath = true;
		bpunchDetach = true;
	}

	return Damage;
}

