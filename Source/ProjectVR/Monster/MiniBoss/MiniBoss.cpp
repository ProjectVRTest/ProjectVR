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
#include "MiniBossParryingPoint.h"
#include "TimerManager.h"

// Sets default values
AMiniBoss::AMiniBoss()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	//GetCapsuleComponent()->SetCollisionObjectType(ECollisionChannel::ECC_GameTraceChannel1);

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

	static ConstructorHelpers::FObjectFinder<UAnimMontage>AttackReverse_Montage(TEXT("AnimMontage'/Game/Blueprints/Monster/MiniBoss/Animation/AM_MB_AttackReverse.AM_MB_AttackReverse'"));

	if (AttackReverse_Montage.Succeeded())
	{
		AttackReverseMontage = AttackReverse_Montage.Object;
	}

	CurrentFalling = false;
	ParryingFlag = false;
	AttackCompleteFlag = false;
	IsParrying = false;
	IsAttack = false; //공격할수 있게 해줌
	StabFlag = false;
	TwoHandWidthFlag = false;
	MaxHP = 300.0f;
	CurrentHP = MaxHP;

	static ConstructorHelpers::FObjectFinder<UMaterialInterface> M_Opacity(TEXT("Material'/Game/Assets/CharacterEquipment/Monster/M_MonsterOpacity.M_MonsterOpacity'"));
	if (M_Opacity.Succeeded())
	{
		OpacityMaterials = M_Opacity.Object;
	}

	static ConstructorHelpers::FObjectFinder<UMaterialInterface> M_DefaultMaterials(TEXT("MaterialInstanceConstant'/Game/Assets/CharacterEquipment/Monster/MiniBoss/Materials/M_MB_Inst.M_MB_Inst'"));
	if (M_DefaultMaterials.Succeeded())
	{
		DefaultMaterials = M_DefaultMaterials.Object;
	}

	AIControllerClass = AMiniBossAIController::StaticClass();

	static ConstructorHelpers::FObjectFinder<UParticleSystem>PT_AfterImageStartEffect(TEXT("ParticleSystem'/Game/Assets/Effect/ES_Skill/PS_StartEffect.PS_StartEffect'"));
	if (PT_AfterImageStartEffect.Succeeded())
	{
		AfterImageStartEffect = PT_AfterImageStartEffect.Object;
	}

	static ConstructorHelpers::FObjectFinder<UParticleSystem>PT_AfterImageEndEffect(TEXT("ParticleSystem'/Game/Assets/Effect/ES_Skill/PS_EndEffect.PS_EndEffect'"));
	if (PT_AfterImageEndEffect.Succeeded())
	{
		AfterImageEndEffect = PT_AfterImageEndEffect.Object;
	}

	static ConstructorHelpers::FObjectFinder<UParticleSystem>PT_InVisibleStartEffect(TEXT("ParticleSystem'/Game/Assets/Effect/ES_Skill/PT_MiniBoss_InVisibleStart.PT_MiniBoss_InVisibleStart'"));
	if (PT_InVisibleStartEffect.Succeeded())
	{
		InVisibleStartEffect = PT_InVisibleStartEffect.Object;
	}


	SwordWaveCount = 1;
	ParryingPointMaxCount = 0;
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
	GetCharacterMovement()->MaxWalkSpeed = 250.0f;
	GetCharacterMovement()->MaxAcceleration = 2048.0f;
	FresnelValue = 1.0f;

	Tags.Add(TEXT("Monster"));
	Tags.Add(FName(TEXT("DisregardForLeftHand")));
	Tags.Add(FName(TEXT("DisregardForRightHand")));
}

// Called when the game starts or when spawned
void AMiniBoss::BeginPlay()
{
	Super::BeginPlay();

	ParryingPointInit();

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
		AI->BBComponent->SetValueAsEnum("CurrentParryingState", (uint8)CurrentParryingState);
		CurrentFalling = GetCharacterMovement()->IsFalling();
	}

	/*FVector TraceEnd = (GetActorLocation()) + (GetActorForwardVector()*-10000.0f);
	TArray<TEnumAsByte<EObjectTypeQuery>>ObjectTypes;

	ObjectTypes.Add(UEngineTypes::ConvertToObjectType(ECollisionChannel::ECC_WorldStatic));

	TArray<AActor*>IgonreActors;
	IgonreActors.Add(this);
	IgonreActors.Add(Sword);

	FHitResult HitResult;

	bool CanSpawn = UKismetSystemLibrary::LineTraceSingleForObjects(
		GetWorld(),
		GetActorLocation()+ FVector(0,0,80.0f),
		TraceEnd,
		ObjectTypes,
		true,
		IgonreActors,
		EDrawDebugTrace::ForDuration,
		HitResult,
		true
	);

	if (CanSpawn)
	{
		float WallDistance = FVector::Distance(GetActorLocation(), HitResult.Location);
		GLog->Log(FString::Printf(TEXT("WallDistance : %f "), WallDistance));
	}*/
}

// Called to bind functionality to input
void AMiniBoss::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

//패링포인트배열에 패링포인트소켓이름을 저장한다.
void AMiniBoss::ParryingPointInit()
{
	ParryingPoints.Add(TEXT("RightArmsParryingPoint"));
	ParryingPoints.Add(TEXT("RightUpperArmsParryingPoint"));
	ParryingPoints.Add(TEXT("LeftArmsParryingPoint"));
	ParryingPoints.Add(TEXT("LeftUpperArmsParryingPoint"));
	ParryingPoints.Add(TEXT("SpineMiddleParryingPoint"));
	ParryingPoints.Add(TEXT("LeftKneeParryingPoint"));
	ParryingPoints.Add(TEXT("RightKneeParryingPoint"));
}

void AMiniBoss::ParryingPointSet()
{
	float HPPercent = CurrentHP / MaxHP;
	int PreviousParryingPointName = -1;
	int RandomParryingPointName;

	AMiniBossParryingPoint * MiniBossParryingPoint;
	FActorSpawnParameters SpawnActorOption;
	SpawnActorOption.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

	FAttachmentTransformRules AttachRules(EAttachmentRule::SnapToTarget, EAttachmentRule::SnapToTarget, EAttachmentRule::KeepWorld, false);

	if (HPPercent > 0.5f && HPPercent <= 1.0f) //HP가 50~100% 일때
	{
		//패링 점을 1~2개 생성하기 위해 1~2만큼 랜덤하게 수를 받는다.
		int RandomParryingPointSpawn = FMath::RandRange(1, 2);

		ParryingPointMaxCount = RandomParryingPointSpawn; //패링 점이 최대 몇개 인지 저장해둔다.

		//위에서 랜덤하게 받은 수만큼 반복하면서
		for (int i = 0; i < ParryingPointMaxCount; i++)
		{
			//7개의 패링 점중에서 랜덤한곳에 스폰시켜 주기 위해 0~6까지의 랜덤수를 받는다.
			RandomParryingPointName = FMath::RandRange(0, 6);

			//이미 스폰한곳은 다시 스폰하지 않기 위해서 그 전랜덤수와 현재 랜덤수를 비교하고
			if (RandomParryingPointName == PreviousParryingPointName)
			{
				i--; //똑같은 수가 있으면 다시 받아서 다른수가 나오게 한다.
				continue;
			}

			//전 랜덤수와 현재 랜덤수가 다르다면 패링점을 월드에 스폰해주고
			MiniBossParryingPoint = GetWorld()->SpawnActor<AMiniBossParryingPoint>(MiniBossParryingPoint->StaticClass(), GetActorLocation(), GetActorRotation(), SpawnActorOption);

			//스폰한 패링점을 붙여줄 위치를 위에서 받은 RandomParryingPointName을 이용해서 ParryingPoint에서 꺼내오고
			FName ParryingPointSpawnLocation = ParryingPoints[RandomParryingPointName];

			//그곳에 앞에서 스폰한 패링포인트를 붙여준다.
			MiniBossParryingPoint->AttachToComponent(GetMesh(), AttachRules, ParryingPointSpawnLocation);

			//전 랜덤수에 현재 랜덤수를 저장해둬서 스폰한곳이 다시 안나오게 해준다.
			PreviousParryingPointName = RandomParryingPointName;
		}
	} //HP가 50%미만 이면 각 HP비율에 맞게 패링 점을 스폰한다.
	else if (HPPercent > 0.4f &&HPPercent <= 0.5f)
	{
		ParryingPointValueSet(3); //3개 랜덤하게 스폰
	}
	else if (HPPercent > 0.3f && HPPercent <= 0.4f)
	{
		ParryingPointValueSet(5); //5개 랜덤하게 스폰
	}
	else if (HPPercent >= 0 && HPPercent <= 0.3f)
	{
		ParryingPointValueSet(7); //7개 랜덤하게 스폰
	}
}

void AMiniBoss::ParryingPointValueSet(int ParryingCount)
{
	int RandomParryingPointName;
	ParryingPointMaxCount = ParryingCount;
	TArray<int32> RandomPointNotOverlap;

	bool RandomFlag;

	for (int k = 0; k < ParryingCount; k++)
	{
		RandomPointNotOverlap.Add(-1);
	}

	AMiniBossParryingPoint * MiniBossParryingPoint;
	FActorSpawnParameters SpawnActorOption;
	SpawnActorOption.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

	FAttachmentTransformRules AttachRules(EAttachmentRule::SnapToTarget, EAttachmentRule::SnapToTarget, EAttachmentRule::KeepWorld, false);

	for (int i = 0; i < ParryingCount; i++)
	{
		RandomParryingPointName = FMath::RandRange(0, 6);
		RandomFlag = true;

		for (int j = 0; j < ParryingCount; j++)
		{
			if (RandomPointNotOverlap[j] == RandomParryingPointName)
			{
				i--;
				RandomFlag = false;
			}
		}

		if (RandomFlag)
		{
			RandomPointNotOverlap[i] = RandomParryingPointName;
		}
	}

	for (int j = 0; j < ParryingCount; j++)
	{
		MiniBossParryingPoint = GetWorld()->SpawnActor<AMiniBossParryingPoint>(MiniBossParryingPoint->StaticClass(), GetActorLocation(), GetActorRotation(), SpawnActorOption);

		if (MiniBossParryingPoint)
		{
			FName ParryingPointSpawnLocation = ParryingPoints[RandomPointNotOverlap[j]];
			MiniBossParryingPoint->AttachToComponent(GetMesh(), AttachRules, ParryingPointSpawnLocation);
		}		
	}

	ParryingPoints.Empty(); //
}

void AMiniBoss::OnSeeCharacter(APawn * Pawn)
{
	if (!Target)
	{
		if (Pawn->ActorHasTag("Character"))
		{
			AMiniBossAIController* AI = Cast<AMiniBossAIController>(GetController());
			if (AI)
			{
				AMotionControllerCharacter * MyCharacter = Cast<AMotionControllerCharacter>(Pawn);

				if (MyCharacter)
				{
					TargetCamera = MyCharacter->CameraLocation;
					AI->BBComponent->SetValueAsObject("PlayerCamera", TargetCamera);
					AI->BBComponent->SetValueAsObject("Player", Pawn);
					Target = Pawn;
					CurrentState = EMiniBossState::Chase;
					CurrentAnimState = EMiniBossAnimState::Walk;
				}
			}
		}
	}
}

void AMiniBoss::Fresnel()
{
	GetMesh()->SetVectorParameterValueOnMaterials(TEXT("Fresnel_color_3"), FVector(1.0f, 0.2f, 0));
	GetMesh()->SetScalarParameterValueOnMaterials(TEXT("Fresnel_exp_3"), FresnelValue);
	FresnelValue += 1.6f;

	if (FresnelValue > 6.0f)
	{
		FresnelValue = 1.0f;
		GetWorld()->GetTimerManager().ClearTimer(MiniBossFresnelTimer);
		GetMesh()->SetVectorParameterValueOnMaterials(TEXT("Fresnel_color_3"), FVector(0, 0, 0));
		GetMesh()->SetScalarParameterValueOnMaterials(TEXT("Fresnel_exp_3"), FresnelValue);
	}
}

float AMiniBoss::TakeDamage(float Damage, FDamageEvent const & DamageEvent, AController * EventInstigator, AActor * DamageCauser)
{
	Super::TakeDamage(Damage, DamageEvent, EventInstigator, DamageCauser);

	CurrentHP -= Damage;

	GetWorld()->GetTimerManager().SetTimer(MiniBossFresnelTimer, this, &AMiniBoss::Fresnel, 0.1f, true, 0.1f);

	if (CurrentHP < 0)
	{
		CurrentHP = 0;
		CurrentState = EMiniBossState::Dead;
	}
	return Damage;
}
