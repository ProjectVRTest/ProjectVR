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
	DogAttackCollision->SetRelativeScale3D(FVector(0.5f, 0.5f, 0.5f));
	DogAttackCollision->SetActive(false);
	DogAttackCollision->ComponentTags.Add("DogAttackCollision");


	PawnSensing->bHearNoises = false;
	PawnSensing->bSeePawns = true;
	PawnSensing->SetPeripheralVisionAngle(40.0f);
	PawnSensing->SightRadius = 1200.0f;
	PawnSensing->SensingInterval = 0.1f;

	bIsAttack = false;
	OnLandFlag = false;

	GetMesh()->SetSimulatePhysics(false);
	GetMesh()->SetCollisionProfileName("Ragdoll");
	GetCharacterMovement()->MaxWalkSpeed = 0.0f;
	Tags.Add("Monster");
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
	DogAttackCollision->OnComponentBeginOverlap.AddDynamic(this, &ADog::OnHeadOverlap);		// 오버랩 이벤트를 발생시킬 수 있도록 설정
}

// Called every frame
void ADog::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	GetCapsuleComponent()->SetRelativeRotation(FRotator(0.0f, GetCapsuleComponent()->GetComponentRotation().Yaw, 0.0f));

	FFindFloorResult FloorDistance;;
	GetCharacterMovement()->ComputeFloorDist(GetCapsuleComponent()->GetComponentLocation(), 10000.0f, 10000.0f, FloorDistance, 34, 0);

	//------------------------------------------------

	AI = Cast<ADogAIController>(GetController());

	if (Target)
	{
		// 뒤쪽을 전후로 60도 차이 이내면 공격 가능 각도
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
		SetActorRelativeRotation(FRotator(0.0f, 0.0f, 0.0f));
		SetActorRelativeLocation(FVector(0.0f, 0.0f, 0.0f));

		FVector ForceVector = GetMesh()->GetPhysicsLinearVelocity() + GetMesh()->GetPhysicsAngularVelocity();

		if (ForceVector.Size() >= 2000.0f)
		{
			DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);
			AttachActor = nullptr;
			bIsAttack = false;

			GetMesh()->SetAllBodiesBelowSimulatePhysics("Bip002-Spine", false, true);
			GetMesh()->SetAllBodiesBelowSimulatePhysics("Bip002-Neck", false, true);
			GetMesh()->SetAllBodiesBelowSimulatePhysics("Bip002-R-Thigh", false, true);
			GetMesh()->SetAllBodiesBelowSimulatePhysics("Bip002-L-Thigh", false, true);
			GetMesh()->SetAllBodiesBelowSimulatePhysics("Bip002-Tail", false, true);

			AMotionControllerCharacter* Character = Cast<AMotionControllerCharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));
			Character->RightHand->AttachDog = nullptr;

			FVector Direction = UKismetMathLibrary::MakeVector(ForceVector.X, ForceVector.Y, ForceVector.Z);

			GetCapsuleComponent()->SetSimulatePhysics(true);
			GetCapsuleComponent()->AddForce(Character->Camera->GetUpVector() * 1000.0f);
			
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
	/*if (OtherActor)
	{
		UE_LOG(LogClass, Warning, TEXT("%s"),*(OtherActor->GetName()));
	}
	
	if(OtherComp)
	{
		UE_LOG(LogClass, Warning, TEXT("%s"), *(OtherComp->GetName()));
	}*/
}

void ADog::OnHeadOverlap(UPrimitiveComponent * OverlappedComp, AActor * OtherActor, UPrimitiveComponent * OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult)
{
	//if (OtherActor)
	//{
	//	UE_LOG(LogClass, Warning, TEXT("--------------------------- %s"), *(OtherActor->GetName()));
	//}

	//if (OtherComp)
	//{
	//	UE_LOG(LogClass, Warning, TEXT("--------------------------- %s"), *(OtherComp->GetName()));
	//}
}

