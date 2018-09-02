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

	//기존에 사용 하던 UAnimBlueprint는 패키징 이후에 엔진이 찾을수 없는것으로 판단
	//새롭게 UClass로 오브젝트를 찾고 꽂아 넣는 형식으로 바꿈
	static ConstructorHelpers::FObjectFinder<UClass>Monster_AnimBlueprint(TEXT("AnimBlueprint'/Game/Blueprints/Monster/Dog/Blueprints2/ABP_Dog.ABP_Dog_C'"));

	if (Monster_AnimBlueprint.Succeeded())
	{
		UClass* DogAnimBlueprint = Monster_AnimBlueprint.Object;
		
		if (DogAnimBlueprint)
		{
			GetMesh()->SetAnimationMode(EAnimationMode::AnimationBlueprint);
			GetMesh()->SetAnimInstanceClass(DogAnimBlueprint);
		}		
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
	GetMesh()->SetRelativeLocation(FVector(-20.0f, 0.0f, -55.0f));
	GetMesh()->SetRelativeRotation(FRotator(0.0f, -90.0f, 0.0f));

	DogAttackCollision->SetCollisionProfileName(TEXT("OverlapAll"));
	DogAttackCollision->SetRelativeLocation(FVector(0.0f, 72.0f, 82.0f));
	DogAttackCollision->SetRelativeScale3D(FVector(0.8f, 0.8f, 0.8f));
	DogAttackCollision->SetActive(false);
	DogAttackCollision->ComponentTags.Add("DogAttackCollision");
	DogAttackCollision->bGenerateOverlapEvents = false;

	PawnSensing->bHearNoises = false;
	PawnSensing->bSeePawns = true;
	PawnSensing->SetPeripheralVisionAngle(40.0f);
	PawnSensing->SightRadius = 1200.0f;
	PawnSensing->SensingInterval = 0.1f;

	bIsAttack = false;
	OnLandFlag = false;		// 물었다 떨어지고 나서 땅에 붙을때 실행되는 것들을 정의
	bOnLand = true;			// 땅에 있는지
	Landing = false;		// 착지인지아닌지

	MaxHP = 1.0f;
	CurrentHP = MaxHP;
	bIsDeath = false;
	bIsDetach = false;

	bInAttackplace = false;

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

	CurrentDogState = EDogState::Idle;
	CurrentDogAnimState = EDogAnimState::Idle;
	CurrentDogJumpState = EDogJumpState::Nothing;
	CurrentDogCircleState = EDogCircleState::Nothing;

	if (PawnSensing)
	{
		PawnSensing->OnSeePawn.AddDynamic(this, &ADog::OnSeePlayer);
	}
}

// Called every frame
void ADog::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	// 틱으로 HP가 0미만인게 확인되면 평시상태로 죽음
	if (CurrentHP < 0.0f && bOnLand)
	{
		DogAttackCollision->bGenerateOverlapEvents = false;
		CurrentDogState = EDogState::Death;
		CurrentDogAnimState = EDogAnimState::StandDeath;

		AMotionControllerCharacter* Character = Cast<AMotionControllerCharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));
		if (Character->DogArray.Contains(this))
			Character->DogArray.Remove(this);
	}

	GetCapsuleComponent()->SetRelativeRotation(FRotator(0.0f, GetCapsuleComponent()->GetComponentRotation().Yaw, 0.0f));

	FFindFloorResult FloorDistance;;
	GetCharacterMovement()->ComputeFloorDist(GetCapsuleComponent()->GetComponentLocation(), 10000.0f, 10000.0f, FloorDistance, 34, 0);

	if (FloorDistance.FloorDist < 3.0f)
		bOnLand = true;
	else
		bOnLand = false;

	AI = Cast<ADogAIController>(GetController());

	if (OnLandFlag && AI)
	{
		if (AI->BBComponent->GetValueAsFloat("DistanceWithLand") < 3.0f)
		{
			// 땅에 붙으면 실행됨 -> 날라가는 액션이 추가되면 #1, #2만 남겨두고 나머지는 태스크 추가해서 실행할 것
			GetMesh()->SetAllBodiesBelowSimulatePhysics("Bip002-Neck", false, true);
			GetCapsuleComponent()->SetSimulatePhysics(false);		// #1
			OnLandFlag = false;		// #2
		}
	}

	// 블랙보드에 매 틱마다 정보 전달
	if (AI)
	{
		AI->BBComponent->SetValueAsEnum("CurrentDogState", (uint8)CurrentDogState);
		AI->BBComponent->SetValueAsEnum("CurrentDogAnimState", (uint8)CurrentDogAnimState);
		AI->BBComponent->SetValueAsEnum("CurrentDogJumpState", (uint8)CurrentDogJumpState);
		AI->BBComponent->SetValueAsEnum("CurrentDogCircleState", (uint8)CurrentDogCircleState);
		AI->BBComponent->SetValueAsFloat("DistanceWithLand", FloorDistance.FloorDist);
		AI->BBComponent->SetValueAsBool("bIsAttack", bIsAttack);
		AI->BBComponent->SetValueAsBool("bHasAttachActor", AttachActor);
		AI->BBComponent->SetValueAsBool("bOnLand", bOnLand);
		AI->BBComponent->SetValueAsBool("Landing", Landing);
		AI->BBComponent->SetValueAsBool("DeathFlag", bIsDeath);
		CurrentFalling = GetCharacterMovement()->IsFalling();
	}

	if (AttachActor)
	{
		// 위치 각도 조정
		SetActorRelativeRotation(FRotator(0.0f, 0.0f, 0.0f));
		SetActorRelativeLocation(FVector(0.0f, 0.0f, 0.0f));

		bOnLand = false;		// 팔에 붙어있으니까 땅에 없음

		FVector ForceVector = GetMesh()->GetPhysicsAngularVelocity();
		AMotionControllerCharacter* Character = Cast<AMotionControllerCharacter>(AttachActor);

		// 포인트 식으로 일정 횟수 누적되면 개가 떨어짐 8이 적당함 - 각도만 틀면 떨어지는것 방지
		if (GetMesh()->GetPhysicsLinearVelocity().Size() >= 300.0f && GetMesh()->GetPhysicsAngularVelocity().Size() >= 400.0f)
		{
			point++;
		}
		else
		{
			if (point > 0 && prelinear < 300 && preangular < 400)		// 전 속도의 최소한계 - GetPhysicsVelocity는 역으로 이동하면 값이 작아짐 -> 전과 비교를해서 낙차가 작으면 포인트 감소
				point--;
		}

		prelinear = GetMesh()->GetPhysicsLinearVelocity().Size();
		preangular = GetMesh()->GetPhysicsAngularVelocity().Size();

		if (point >= 8 || bpunchDetach)
		{
			CurrentDogState = EDogState::Hurled;
			CurrentDogAnimState = EDogAnimState::Nothing;
			CurrentDogJumpState = EDogJumpState::Nothing;
			CurrentDogCircleState = EDogCircleState::Nothing;

			point = 0;
			DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);		// 뗌
			AttachActor = nullptr;		// 개의 붙은 액터 초기화
			bIsAttack = false;			// 공격상태 아님/
			bpunchDetach = false;
			bIsDetach = true;

			// 캐릭터의 오른손의 붙어있는 액터를 초기화
			AMotionControllerCharacter* Character = Cast<AMotionControllerCharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));
			if (Character)
			{
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

		if ( AI && !AI->BBComponent->GetValueAsObject("Player"))
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


float ADog::TakeDamage(float Damage, FDamageEvent const & DamageEvent, AController * EventInstigator, AActor * DamageCauser)
{
	APlayerSword* Sword = Cast<APlayerSword>(DamageCauser);
	if (CurrentDogState == EDogState::Bite && Sword)
		return 0;

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
	}

	return Damage;
}

