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

	static ConstructorHelpers::FObjectFinder<USkeletalMesh>MonsterMesh(TEXT("SkeletalMesh'/Game/Assets/Monster/Dog/Mesh2/MON_DOG_MESH.MON_DOG_MESH'"));
	if (MonsterMesh.Succeeded())
	{
		GetMesh()->SetSkeletalMesh(MonsterMesh.Object);
	}
	static ConstructorHelpers::FObjectFinder<UBehaviorTree>Monster_BehaviorTree(TEXT("BehaviorTree'/Game/Blueprints/Monster/Dog/AI/RagdollDogBT.RagdollDogBT'"));
	if (Monster_BehaviorTree.Succeeded())
	{
		BehaviorTree = Monster_BehaviorTree.Object;
	}

	static ConstructorHelpers::FObjectFinder<UAnimBlueprint>Monster_AnimBlueprint(TEXT("AnimBlueprint'/Game/Blueprints/Monster/Dog/Blueprints2/ABP_Dog.ABP_Dog'"));

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

	MaxHP = 1.0f;
	CurrentHP = MaxHP;
	bIsDeath = false;

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

	CurrentDogState = EDogState::Idle;
	CurrentDogAnimState = EDogAnimState::Idle;
	CurrentDogJumpState = EDogJumpState::Nothing;
	CurrentDogCircleState = EDogCircleState::Nothing;

	if (PawnSensing)
	{
		PawnSensing->OnSeePawn.AddDynamic(this, &ADog::OnSeePlayer);
	}

	GetMesh()->OnComponentBeginOverlap.AddDynamic(this, &ADog::OnBodyOverlap);
	DogAttackCollision->OnComponentBeginOverlap.AddDynamic(this, &ADog::OnHeadOverlap);		// 오버랩 이벤트를 발생시킬 수 있도록 설정
}

// Called every frame
void ADog::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	/*if (CurrentDogState == EDogState::Battle)
	{
		UE_LOG(LogTemp, Log, TEXT("Battle"));
	}
	else if (CurrentDogState == EDogState::Chase)
	{
		UE_LOG(LogTemp, Log, TEXT("Chase  %f"), GetCharacterMovement()->MaxWalkSpeed);
	}
	else if (CurrentDogState == EDogState::Circle)
	{
		UE_LOG(LogTemp, Log, TEXT("Circle"));
	}
	else if (CurrentDogState == EDogState::Death)
	{
		UE_LOG(LogTemp, Log, TEXT("Death"));
	}
	else if (CurrentDogState == EDogState::Hurled)
	{
		UE_LOG(LogTemp, Log, TEXT("Hurled"));
	}
	else if (CurrentDogState == EDogState::Idle)
	{
		UE_LOG(LogTemp, Log, TEXT("Idle"));
	}
	else if (CurrentDogState == EDogState::Nothing)
	{
		UE_LOG(LogTemp, Log, TEXT("Nothing"));
	}*/

	/*if (CurrentDogAnimState == EDogAnimState::SideWalk)
	{
		UE_LOG(LogTemp, Log, TEXT("SideWalk"));

		if (CurrentDogCircleState == EDogCircleState::LeftCircle)
		{
			UE_LOG(LogTemp, Log, TEXT("LeftCircle"));
		}
		else if (CurrentDogCircleState == EDogCircleState::Nothing)
		{
			UE_LOG(LogTemp, Log, TEXT("Nothing"));
		}
		else if (CurrentDogCircleState == EDogCircleState::RightCircle)
		{
			UE_LOG(LogTemp, Log, TEXT("RightCircle"));
		}
	}
	else if (CurrentDogAnimState == EDogAnimState::JumpAttack)
	{
		UE_LOG(LogTemp, Log, TEXT("JumpAttack"));

		if (CurrentDogJumpState == EDogJumpState::JumpStart)
		{
			UE_LOG(LogTemp, Log, TEXT("JumpStart"));
		}
		else if (CurrentDogJumpState == EDogJumpState::Nothing)
		{
			UE_LOG(LogTemp, Log, TEXT("Nothing"));
		}
		else if (CurrentDogJumpState == EDogJumpState::JumpEnd)
		{
			UE_LOG(LogTemp, Log, TEXT("JumpEnd"));
		}
		else if (CurrentDogJumpState == EDogJumpState::JumpRoof)
		{
			UE_LOG(LogTemp, Log, TEXT("JumpRoof"));
		}
	}
	else if(CurrentDogAnimState == EDogAnimState::Nothing)
	{
		UE_LOG(LogTemp, Log, TEXT("Nothing"));
	}
	else if (CurrentDogAnimState == EDogAnimState::Idle)
	{
		UE_LOG(LogTemp, Log, TEXT("Idle"));
	}
	else if (CurrentDogAnimState == EDogAnimState::Run)
	{
		UE_LOG(LogTemp, Log, TEXT("Run"));
	}*/	

	GetCapsuleComponent()->SetRelativeRotation(FRotator(0.0f, GetCapsuleComponent()->GetComponentRotation().Yaw, 0.0f));

	FFindFloorResult FloorDistance;;
	GetCharacterMovement()->ComputeFloorDist(GetCapsuleComponent()->GetComponentLocation(), 10000.0f, 10000.0f, FloorDistance, 34, 0);

	AI = Cast<ADogAIController>(GetController());

	if (OnLandFlag)
	{
		if (AI->BBComponent->GetValueAsFloat("DistanceWithLand") < 3.0f)
		{
			// 땅에 붙으면 실행됨 -> 날라가는 액션이 추가되면 #1, #2만 남겨두고 나머지는 태스크 추가해서 실행할 것
			GetMesh()->SetAllBodiesBelowSimulatePhysics("Bip002-Spine", false, true);
			GetMesh()->SetAllBodiesBelowSimulatePhysics("Bip002-Neck", false, true);
			GetMesh()->SetAllBodiesBelowSimulatePhysics("Bip002-R-Thigh", false, true);
			GetMesh()->SetAllBodiesBelowSimulatePhysics("Bip002-L-Thigh", false, true);
			GetMesh()->SetAllBodiesBelowSimulatePhysics("Bip002-Tail", false, true);
			GetCapsuleComponent()->SetSimulatePhysics(false);		// #1
			OnLandFlag = false;		// #2
		}
	}

	if (AI)
	{
		AI->BBComponent->SetValueAsEnum("CurrentDogState", (uint8)CurrentDogState);
		AI->BBComponent->SetValueAsEnum("CurrentDogAnimState", (uint8)CurrentDogAnimState);
		AI->BBComponent->SetValueAsEnum("CurrentDogJumpState", (uint8)CurrentDogJumpState);
		AI->BBComponent->SetValueAsEnum("CurrentDogCircleState", (uint8)CurrentDogCircleState);
		AI->BBComponent->SetValueAsFloat("DistanceWithLand", FloorDistance.FloorDist);

		CurrentFalling = GetCharacterMovement()->IsFalling();
	}

	if (AttachActor)
	{
		// 위치 각도 조정
		SetActorRelativeRotation(FRotator(0.0f, 0.0f, 0.0f));
		SetActorRelativeLocation(FVector(0.0f, 0.0f, 0.0f));

		FVector ForceVector = GetMesh()->GetPhysicsAngularVelocity();
		AMotionControllerCharacter* Character = Cast<AMotionControllerCharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));

		// 포인트 식으로 일정 횟수 누적되면 개가 떨어짐 8이 적당함 - 각도만 틀면 떨어지는것 방지
		if (GetMesh()->GetPhysicsLinearVelocity().Size() >= 300.0f && GetMesh()->GetPhysicsAngularVelocity().Size() >= 400.0f)
		{
			point++;
		}
		else
		{
			if (prelinear < 300 && preangular < 400)		// 전 속도의 최소한계 - GetPhysicsVelocity는 역으로 이동하면 값이 작아짐 -> 전과 비교를해서 낙차가 작으면 포인트 감소
				point--;
		}

		prelinear = GetMesh()->GetPhysicsLinearVelocity().Size();
		preangular = GetMesh()->GetPhysicsAngularVelocity().Size();

		if (point >= 8 || bpunchDetach)
		{
			point = 0;
			DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);		// 뗌
			AttachActor = nullptr;		// 개의 붙은 액터 초기화
			bIsAttack = false;			// 공격상태 아님/
			bpunchDetach = false;
			// 캐릭터의 오른손의 붙어있는 액터를 초기화
			AMotionControllerCharacter* Character = Cast<AMotionControllerCharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));
			Character->RightHand->AttachDog = nullptr;

			// 날라가는 방향
			FVector Direction = Character->Camera->GetUpVector() + Character->Camera->GetForwardVector();

			// 날라가는 힘을 조절
			GetCapsuleComponent()->SetPhysicsLinearVelocity(Direction* 500.0f);
			GetCapsuleComponent()->SetPhysicsAngularVelocity(Direction* 500.0f);
			GetCapsuleComponent()->SetSimulatePhysics(true);
			GetCapsuleComponent()->AddForce(Direction * 500.0f);

			OnLandFlag = true;		// 바닥에 닿았을 때 한번만 실행
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
			CurrentDogCircleState = EDogCircleState::Nothing;
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
	
}

float ADog::TakeDamage(float Damage, FDamageEvent const & DamageEvent, AController * EventInstigator, AActor * DamageCauser)
{
	CurrentHP -= Damage;

	if (CurrentHP < 0.0f)
	{
		bIsDeath = true;
		bpunchDetach = true;
		UE_LOG(LogTemp, Log, TEXT("Death!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!"));
	}

	return Damage;
}

