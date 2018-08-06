// Fill out your copyright notice in the Description page of Project Settings.

#include "MotionControllerCharacter.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "Components/SceneComponent.h"

#include "GameFramework/CharacterMovementComponent.h"

#include "HandMotionController/LeftHandMotionController.h"
#include "HandMotionController/RightHandMotionController.h"

#include "Engine/World.h"
#include "Components/SkeletalMeshComponent.h"

#include "Public/TimerManager.h" 
#include "Equipment/PlayerShield.h"
#include "Components/WidgetComponent.h"
#include "HandMotionController/Widget/LeftHandWidget.h"

#include "Components/StereoLayerComponent.h"
#include "Runtime/CoreUObject/Public/UObject/ConstructorHelpers.h"
#include "Runtime/Engine/Classes/Engine/TextureRenderTarget2D.h"

#include "MyCharacter/Widget/HitBloodyWidget.h"

#include "Components/StaticMeshComponent.h"
#include "Components/BoxComponent.h"

#include "Equipment/PlayerSword.h"

#include "MyTargetPoint.h"

#include "Monster/Dog/Dog.h"
#include "Components/CapsuleComponent.h"
#include "Components/SphereComponent.h"

// Sets default values
AMotionControllerCharacter::AMotionControllerCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	bUseControllerRotationYaw = true;
	
	SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm"));
	SpringArm->SetupAttachment(RootComponent);

	SpringArm->bUsePawnControlRotation = true;
	SpringArm->bInheritPitch = true;
	SpringArm->bInheritYaw = true;
	SpringArm->bInheritRoll = false;
	SpringArm->TargetArmLength = 1.0f;

	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	Camera->SetupAttachment(SpringArm);

	Scene = CreateDefaultSubobject<USceneComponent>(TEXT("SceneA"));
	Scene->SetupAttachment(Camera);

	Stereo = CreateDefaultSubobject<UStereoLayerComponent>(TEXT("StereoB"));
	Stereo->SetupAttachment(Scene);

	Widget = CreateDefaultSubobject<UWidgetComponent>(TEXT("WidgetB"));
	Widget->SetupAttachment(Scene);

	Stereo->SetRelativeLocation(FVector(70.0f, 0.0f, 0.0f));
	Stereo->bLiveTexture = true;
	Stereo->SetQuadSize(FVector2D(1000.0f, 1000.0f));
	static ConstructorHelpers::FClassFinder<UUserWidget> HitUI(TEXT("WidgetBlueprint'/Game/Blueprints/UI/BloodEffectHUD.BloodEffectHUD_C'"));
	if (HitUI.Succeeded())
	{
		Widget->SetWidgetClass(HitUI.Class);
	}
	Widget->SetWidgetSpace(EWidgetSpace::World);
	Widget->SetDrawSize(FVector2D(1000.0f, 1000.0f));
	Widget->bVisible = false;

	HeadBox = CreateDefaultSubobject<UBoxComponent>(TEXT("HeadBox"));
	HeadBox->SetRelativeScale3D(FVector(0.4f, 0.4f, 0.4f));
	HeadBox->SetupAttachment(Camera);
	HeadBox->SetCollisionProfileName(TEXT("OverlapAll"));
	HeadBox->bGenerateOverlapEvents = true;
	HeadBox->ComponentTags.Add(FName("Head"));

	GetCharacterMovement()->MaxWalkSpeed = 450.0f;
	
	MaxHp = 100.0f;
	CurrentHp = 100.0f;
	MaxStamina = 100.0f;
	CurrentStamina = MaxStamina;

	InvincibleTimeOn = false;
	CurrentState = EPlayerState::Idle;
	bAllowBreathe = true;
	DashPower = 800.0f;
	GrabState = E_HandState::Open;		// ���߿� ���� ����ȭ ó���ϸ� �׷����·� �ٲ����

	Tags.Add(FName("Character"));
	Tags.Add(FName(TEXT("DisregardForLeftHand")));
	Tags.Add(FName(TEXT("DisregardForRightHand")));
}

// Called when the game starts or when spawned
void AMotionControllerCharacter::BeginPlay()
{
	Super::BeginPlay();
	
	FActorSpawnParameters SpawnActorOption;
	SpawnActorOption.Owner = this;
	SpawnActorOption.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

	FAttachmentTransformRules AttachRules(EAttachmentRule::SnapToTarget, EAttachmentRule::SnapToTarget, EAttachmentRule::KeepWorld, false);

	LeftHand = GetWorld()->SpawnActor<ALeftHandMotionController>(LeftHand->StaticClass(), GetMesh()->GetComponentLocation(), GetMesh()->GetComponentRotation(), SpawnActorOption);

	LeftHand->AttachToComponent(GetMesh(), AttachRules);

	RightHand = GetWorld()->SpawnActor<ARightHandMotionController>(RightHand->StaticClass(), GetMesh()->GetComponentLocation(), GetMesh()->GetComponentRotation(), SpawnActorOption);
	RightHand->AttachToComponent(GetMesh(), AttachRules);

	if (HeadBox)
	{
		HeadBox->OnComponentBeginOverlap.AddDynamic(this, &AMotionControllerCharacter::OnHeadOverlap);		// ������ �̺�Ʈ�� �߻���ų �� �ֵ��� ����
	}

	AttackPointSet();
}

// Called every frame
void AMotionControllerCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	float StandardAngle = Camera->GetComponentRotation().Yaw + 180.0f;

	float Max = StandardAngle + 40.0f;
	float Min = Max - 80.0f;

	Max = Max >= 360.0f ? Max - 360.0f : Max;
	Min = Min < 0.0f ? 360.0f + Min : Min;

	Max += 180.0;
	Min += 180.0;

	Max = Max >= 360.0f ? Max - 360.0f : Max;
	Min = Min >= 360.0f ? Min - 360.0f : Min;

	//UE_LOG(LogClass, Warning, TEXT("Left2 ------ %f / %f / %f"), StandardAngle, Min, Max);
	//if (SpringArm)
	//{
	//	if (bAllowBreathe)
	//	{
	//		FVector newLocation = SpringArm->RelativeLocation;
	//		float loca = (FMath::Sin(RunningTime + DeltaTime) - FMath::Sin(RunningTime));
	//		newLocation.Z += loca * 10.f;
	//		RunningTime += (DeltaTime * 5);

	//		SpringArm->SetRelativeLocation(newLocation);
	//	}
	//}

	//if (IsValid(Widget->GetRenderTarget()))
	//{
	//	UTexture* texture;
	//	texture = Cast<UTextureRenderTarget2D>(Widget->GetRenderTarget());
	//	Stereo->SetTexture(Widget->GetRenderTarget());
	//}

	//// Idle ����, �������� ���� �� ī�޶� ������ ��鸲.
	//if (CurrentState != EPlayerState::Idle && this->GetVelocity().Size() == 0)
	//{
	//	CurrentState = EPlayerState::Idle;
	//	GetWorld()->GetTimerManager().SetTimer(SetIdleTimerHandle, this, &AMotionControllerCharacter::SetAllowBreathe, 0.01f, false, 1.0f);		// 1.5�� �� �����ð��� ��Ȱ��ȭ
	//}
	//else if (this->GetVelocity().Size() > 0)
	//{
	//	bAllowBreathe = false;
	//}		
}

// Called to bind functionality to input
void AMotionControllerCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAction(TEXT("GrabLeft"), IE_Pressed, this, &AMotionControllerCharacter::GrabLeftOn);
	PlayerInputComponent->BindAction(TEXT("GrabLeft"), IE_Released, this, &AMotionControllerCharacter::GrabLeftOff);

	PlayerInputComponent->BindAction(TEXT("GrabRight"), IE_Pressed, this, &AMotionControllerCharacter::GrabRightOn);
	PlayerInputComponent->BindAction(TEXT("GrabRight"), IE_Released, this, &AMotionControllerCharacter::GrabRightOff);

	PlayerInputComponent->BindAction(TEXT("Run"), IE_Pressed, this, &AMotionControllerCharacter::RunOn);
	PlayerInputComponent->BindAction(TEXT("Run"), IE_Released, this, &AMotionControllerCharacter::RunOff);

	PlayerInputComponent->BindAxis(TEXT("MoveForward"), this, &AMotionControllerCharacter::MoveForward);
	PlayerInputComponent->BindAxis(TEXT("MoveRight"), this, &AMotionControllerCharacter::MoveRight);
	
	PlayerInputComponent->BindAction(TEXT("DashUp"), IE_Pressed, this, &AMotionControllerCharacter::DashUpStart);
	PlayerInputComponent->BindAction(TEXT("DashUp"), IE_Released, this, &AMotionControllerCharacter::DashEnd);

	PlayerInputComponent->BindAction(TEXT("DashDown"), IE_Pressed, this, &AMotionControllerCharacter::DashDownStart);
	PlayerInputComponent->BindAction(TEXT("DashDown"), IE_Released, this, &AMotionControllerCharacter::DashEnd);

	PlayerInputComponent->BindAction(TEXT("DashLeft"), IE_Pressed, this, &AMotionControllerCharacter::DashLeftStart);
	PlayerInputComponent->BindAction(TEXT("DashLeft"), IE_Released, this, &AMotionControllerCharacter::DashEnd);

	PlayerInputComponent->BindAction(TEXT("DashRight"), IE_Pressed, this, &AMotionControllerCharacter::DashRightStart);
	PlayerInputComponent->BindAction(TEXT("DashRight"), IE_Released, this, &AMotionControllerCharacter::DashEnd);
}

// ���Ľ�Ƽ���� �־ �׻� �׷����·� �ִ´ٰ� ������ ��
void AMotionControllerCharacter::GrabLeftOn()
{
	// �޼����� �Ҽ� �ִ°��� �ƹ��͵� �����Ƿ� LeftHand->GrabActor();�� ���°� ����ؾ���, �׷��� ���� �μ����� ���ٰ��ϸ� ���ǹ� �༭ ���� �ƴҶ��� �ɷ������

	GrabState = E_HandState::Grab;

	// GrabActor�� ���� �޼��� ���� ���� �����ؼ� ���ο� �Լ� �����
	LeftHand->GrabActor();

	// if(������ ���̸�)
	// ������ �ۿ��ϴ� �Լ�

	// ó���� �׷����¸� State::Grab���� �����ؾ���, GrabActor()�Լ����� ��ȯ�� bool�� ��ȯ�ؾ��� -> ���� Ȯ������ �����Ƿ� ���� �׽�Ʈ
	// if(������ ������) 
	LeftHand->Shield->ConvertOfOpacity(1.0);
}

void AMotionControllerCharacter::GrabLeftOff()
{
	GrabState = E_HandState::Open;
	LeftHand->ReleaseActor();
	LeftHand->Shield->ConvertOfOpacity(0.75);
}

void AMotionControllerCharacter::GrabRightOn()
{
	GrabState = E_HandState::Grab;

	RightHand->GrabActor();
	
	RightHand->Sword->ConvertOfOpacity(1.0);
}

void AMotionControllerCharacter::GrabRightOff()
{
	GrabState = E_HandState::Open;

	RightHand->ReleaseActor();

	RightHand->Sword->ConvertOfOpacity(0.75);
}

void AMotionControllerCharacter::MoveForward(float Value)
{
	if (Value != 0)
	{
		CurrentState = EPlayerState::Walk;
		AddMovementInput(Camera->GetForwardVector(), Value);
	}
}

void AMotionControllerCharacter::MoveRight(float Value)
{
	if (Value != 0)
	{
		CurrentState = EPlayerState::Walk;
		AddMovementInput(Camera->GetRightVector(), Value);
	}
}

void AMotionControllerCharacter::RunOn()
{
	CurrentState = EPlayerState::Run;
	GetCharacterMovement()->MaxWalkSpeed = 675.0f;
}

void AMotionControllerCharacter::RunOff()
{
	CurrentState = EPlayerState::Run;
	GetCharacterMovement()->MaxWalkSpeed = 450.0f;
}

void AMotionControllerCharacter::DashUpStart()
{
	FVector DashVector = FVector::ZeroVector;
	GetCharacterMovement()->GroundFriction = 0;
	DashVector = Camera->GetForwardVector()*3000.0f;
	DashVector.Z = 0;
	LaunchCharacter(DashVector,false,false);
}

void AMotionControllerCharacter::DashDownStart()
{
	FVector DashVector = FVector::ZeroVector;
	GetCharacterMovement()->GroundFriction = 0;
	DashVector = Camera->GetForwardVector()*3000.0f*-1.0f;
	DashVector.Z = 0;
	LaunchCharacter(DashVector, false, false);
}

void AMotionControllerCharacter::DashLeftStart()
{
	FVector DashVector = FVector::ZeroVector;
	GetCharacterMovement()->GroundFriction = 0;
	DashVector = Camera->GetRightVector()*3000.0f*-1.0f;
	DashVector.Z = 0;
	LaunchCharacter(DashVector, false, false);
}

void AMotionControllerCharacter::DashRightStart()
{
	FVector DashVector = FVector::ZeroVector;
	GetCharacterMovement()->GroundFriction = 0;
	DashVector = Camera->GetRightVector()*3000.0f;
	DashVector.Z = 0;
	LaunchCharacter(DashVector, false, false);
}

void AMotionControllerCharacter::DashEnd()
{
	GetCharacterMovement()->GroundFriction = 8.0f;
}

void AMotionControllerCharacter::AttackPointSet()
{
	AMyTargetPoint* AttackPoint;
	FAttachmentTransformRules AttachRules(EAttachmentRule::KeepWorld, EAttachmentRule::KeepWorld, EAttachmentRule::KeepWorld, false);
	FVector CalculatePoint;
	FVector InitPoint =	Camera->GetComponentLocation();
	FVector Point;

	CalculatePoint = InitPoint;
	CalculatePoint.X = InitPoint.X + 200.0f;
	Point = CalculatePoint;
	Point.Z = Point.Z - 248.0f;

	AttackPoint = GetWorld()->SpawnActor<AMyTargetPoint>(AttackPoint->StaticClass(), Point, this->GetActorRotation());
	AttackPoints.Add(AttackPoint);
	AttackPoint->AttachToComponent(Camera, AttachRules);

	CalculatePoint.X = InitPoint.X - 200.0f;
	Point = CalculatePoint;
	Point.Z = Point.Z - 248.0f;
	AttackPoint = GetWorld()->SpawnActor<AMyTargetPoint>(AttackPoint->StaticClass(), Point, this->GetActorRotation());
	AttackPoints.Add(AttackPoint);
	AttackPoint->AttachToComponent(Camera, AttachRules);

	CalculatePoint = InitPoint;
	CalculatePoint.Y = InitPoint.Y - 200.0f;
	Point = CalculatePoint;
	Point.Z = Point.Z - 248.0f;
	AttackPoint = GetWorld()->SpawnActor<AMyTargetPoint>(AttackPoint->StaticClass(), Point, this->GetActorRotation());
	AttackPoints.Add(AttackPoint);
	AttackPoint->AttachToComponent(Camera, AttachRules);

	CalculatePoint.Y = InitPoint.Y + 200.0f;
	Point = CalculatePoint;
	Point.Z = Point.Z - 248.0f;
	AttackPoint = GetWorld()->SpawnActor<AMyTargetPoint>(AttackPoint->StaticClass(), Point, this->GetActorRotation());
	AttackPoints.Add(AttackPoint);
	AttackPoint->AttachToComponent(Camera, AttachRules);

	CalculatePoint = InitPoint;
	CalculatePoint.X = InitPoint.X + 200.0f;
	CalculatePoint.Y = InitPoint.Y - 200.0f;
	Point = CalculatePoint;
	Point.Z = Point.Z - 248.0f;
	AttackPoint = GetWorld()->SpawnActor<AMyTargetPoint>(AttackPoint->StaticClass(), Point, this->GetActorRotation());
	AttackPoints.Add(AttackPoint);
	AttackPoint->AttachToComponent(Camera, AttachRules);

	CalculatePoint = InitPoint;
	CalculatePoint.X = InitPoint.X + 200.0f;
	CalculatePoint.Y = InitPoint.Y + 200.0f;
	Point = CalculatePoint;
	Point.Z = Point.Z - 248.0f;
	AttackPoint = GetWorld()->SpawnActor<AMyTargetPoint>(AttackPoint->StaticClass(), Point, this->GetActorRotation());
	AttackPoints.Add(AttackPoint);
	AttackPoint->AttachToComponent(Camera, AttachRules);

	CalculatePoint = InitPoint;
	CalculatePoint.X = InitPoint.X - 200.0f;
	CalculatePoint.Y = InitPoint.Y - 200.0f;
	Point = CalculatePoint;
	Point.Z = Point.Z - 248.0f;
	AttackPoint = GetWorld()->SpawnActor<AMyTargetPoint>(AttackPoint->StaticClass(), Point, this->GetActorRotation());
	AttackPoints.Add(AttackPoint);
	AttackPoint->AttachToComponent(Camera, AttachRules);

	CalculatePoint = InitPoint;
	CalculatePoint.X = InitPoint.X - 200.0f;
	CalculatePoint.Y = InitPoint.Y + 200.0f;
	Point = CalculatePoint;
	Point.Z = Point.Z - 248.0f;
	AttackPoint = GetWorld()->SpawnActor<AMyTargetPoint>(AttackPoint->StaticClass(), Point, this->GetActorRotation());
	AttackPoints.Add(AttackPoint);
	AttackPoint->AttachToComponent(Camera, AttachRules);
}

void AMotionControllerCharacter::SetAllowBreathe()
{
	bAllowBreathe = true;			// Ÿ�̸ӷ� �������� �� �� �Ŀ� �ٽ� ������ �ְ� ��
}


float AMotionControllerCharacter::TakeDamage(float Damage, FDamageEvent const & DamageEvent, AController * EventInstigator, AActor * DamageCauser)
{

	if (!InvincibleTimeOn)
	{
		bisHit = true;
		Widget->bVisible = true;

		if (Widget->bVisible)		// �ǰ� ������ Ȱ��ȭ �ɶ� ����
		{
			UHitBloodyWidget* bloodyWidget = Cast<UHitBloodyWidget>(Widget->GetUserWidgetObject());		// UHitBloodyWidget�Լ��� ����Ҽ� �ְ� ��
			if (bloodyWidget)
			{
				bloodyWidget->PlayAnimationByName("Bloody",0.0, 1,EUMGSequencePlayMode::Forward, 1.0f);		// �ִϸ��̼� ����
			}
		}

		CurrentHp -= Damage;			// ���� ü�°���
		ULeftHandWidget* HandWidget = Cast<ULeftHandWidget>
			(LeftHand->Shield->CharacterStateWidget->GetUserWidgetObject());		// �޼� ������ ������ ULeftHandWidget���� �Լ��� ����� �� �ֵ��� ĳ��Ʈ�Ѵ�.

		if (HandWidget)
		{
			HandWidget->ReceiveDamage(Damage);			// �������� �޴´�.
		}
		InvincibleTimeOn = true;		// �ǰݵǸ� ��� �����ð� Ȱ��ȭ
		GetWorld()->GetTimerManager().SetTimer(DamageTimerHandle, this, &AMotionControllerCharacter::DamageTimer, 0.01f, false, 1.5f);		// 1.5�� �� �����ð��� ��Ȱ��ȭ
	}
	
	return Damage;
}

void AMotionControllerCharacter::DamageTimer()
{
	InvincibleTimeOn = false;			// �����ð� ��Ȱ��ȭ
}

bool AMotionControllerCharacter::PlayBloodyOverlay()
{
	if (bisHit)
	{
		bisHit = false;
		return true;
	}
	return false;
}

void AMotionControllerCharacter::DisableBloody()		
{
	if (Widget->bVisible)		
		Widget->bVisible = false;		// ������ ������ �ʰ� ��
}

void AMotionControllerCharacter::OnHeadOverlap(UPrimitiveComponent * OverlappedComp, AActor * OtherActor, UPrimitiveComponent * OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult)
{
	if (OtherActor->ActorHasTag("Potion") && GrabState == E_HandState::Grab)		// ������Ʈ ����, ���� �����̸� ù��° ���� OtherActor->ActorHasTas("Potion") ���� ��ȯ
	{
		CurrentHp += 30;		// ȸ����

		ULeftHandWidget* HandWidget = Cast<ULeftHandWidget>
			(LeftHand->Shield->CharacterStateWidget->GetUserWidgetObject());		// �޼� ������ ������ ULeftHandWidget���� �Լ��� ����� �� �ֵ��� ĳ��Ʈ�Ѵ�.

		if (HandWidget)
		{
			HandWidget->GainHP(30);		// ȸ��
		}
	}

	if (OtherComp->ComponentHasTag("DogAttackCollision"))
	{

		ARightHandMotionController* RightController = Cast<ARightHandMotionController>(RightHand);
		
		if (!RightController->AttachDog)
		{
			ADog* Dog = Cast<ADog>(OtherActor);
			if (Dog)
			{
				UE_LOG(LogClass, Warning, TEXT("dddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddd") );

				RightController->AttachDog = Dog;
				Dog->bIsAttack = true;
				Dog->DogAttackCollision->SetActive(false);

				// KeepRelative : ���� ������ ������ �ٴ� ������ ������
				FAttachmentTransformRules AttachRules(EAttachmentRule::SnapToTarget, EAttachmentRule::SnapToTarget, EAttachmentRule::KeepWorld, true);

				Dog->AttachToComponent(RightController->AttachDogPosition, AttachRules);
				Dog->GetMesh()->SetAllBodiesBelowSimulatePhysics("Bip002-Spine", true, true);
				Dog->GetMesh()->SetAllBodiesBelowSimulatePhysics("Bip002-Neck", false, true);
				Dog->GetMesh()->SetAllBodiesBelowSimulatePhysics("Bip002-R-Thigh", true, true);
				Dog->GetMesh()->SetAllBodiesBelowSimulatePhysics("Bip002-L-Thigh", true, true);
				Dog->GetMesh()->SetAllBodiesBelowSimulatePhysics("Bip002-Tail", true, true);

				Dog->SetActorRelativeLocation(FVector(0.0f, 0.0f, 0.0f));
				Dog->SetActorRelativeRotation(FRotator(0.0f, 0.0f, 0.0f));
				//Dog->GetCharacterMovement()->DisableMovement();
				//Dog->GetCharacterMovement()->MovementMode = EMovementMode::MOVE_None;
				Dog->AttachActor = this;
			}
		}
	}
}
