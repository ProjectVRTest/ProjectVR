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
#include "HeadMountedDisplayFunctionLibrary.h"
#include "MyCharacter/Widget/HPStaminaBar.h"

#include "MyCharacter/Widget/Menu.h"

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

	Stereo = CreateDefaultSubobject<UStereoLayerComponent>(TEXT("StereoB"));
	Stereo->SetupAttachment(Camera);

	Widget = CreateDefaultSubobject<UWidgetComponent>(TEXT("WidgetB"));
	Widget->SetupAttachment(Camera);

	Stereo->SetRelativeLocation(FVector(100.0f, 0.0f, 0.0f));
	Stereo->bLiveTexture = true;
	Stereo->SetQuadSize(FVector2D(250.0f, 250.0f));

	static ConstructorHelpers::FClassFinder<UUserWidget> HitUI(TEXT("WidgetBlueprint'/Game/Blueprints/UI/BloodEffectHUD.BloodEffectHUD_C'"));
	if (HitUI.Succeeded())
	{
		Widget->SetWidgetClass(HitUI.Class);
	}
	
	Widget->SetWidgetSpace(EWidgetSpace::World);
	Widget->SetDrawSize(FVector2D(1000.0f, 1000.0f));
	Widget->bVisible = true;

	HeadBox = CreateDefaultSubobject<UBoxComponent>(TEXT("HeadBox"));
	HeadBox->SetRelativeScale3D(FVector(0.4f, 0.4f, 0.4f));
	HeadBox->SetupAttachment(Camera);
	HeadBox->SetCollisionProfileName(TEXT("OverlapAll"));
	HeadBox->bGenerateOverlapEvents = true;
	HeadBox->ComponentTags.Add(FName("Head"));

	GetCharacterMovement()->MaxWalkSpeed = 280.0f;

	MaxHp = 100.0f;
	CurrentHp = MaxHp;
	MaxStamina = 100.0f;
	CurrentStamina = MaxStamina;

	InvincibleTimeOn = false;
	CurrentState = EPlayerState::Idle;
	bAllowBreathe = true;
	DashPower = 800.0f;
	GrabState = E_HandState::Open;		// 나중에 무기 투명화 처리하면 그랩상태로 바꿔야함

	Tags.Add(FName("Character"));
	Tags.Add(FName(TEXT("DisregardForLeftHand")));
	Tags.Add(FName(TEXT("DisregardForRightHand")));
}

// Called when the game starts or when spawned
void AMotionControllerCharacter::BeginPlay()
{
	Super::BeginPlay();

	FName DeviceName = UHeadMountedDisplayFunctionLibrary::GetHMDDeviceName();
	
	GLog->Log(DeviceName.ToString());
	if (DeviceName == "SteamVR" || DeviceName == "OculusHMD")
	{
		UHeadMountedDisplayFunctionLibrary::SetTrackingOrigin(EHMDTrackingOrigin::Eye);
	}

	FActorSpawnParameters SpawnActorOption;
	SpawnActorOption.Owner = this;
	SpawnActorOption.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

	FAttachmentTransformRules AttachRules(EAttachmentRule::SnapToTarget, EAttachmentRule::SnapToTarget, EAttachmentRule::KeepWorld, false);
	
	LeftHand = GetWorld()->SpawnActor<ALeftHandMotionController>(LeftHand->StaticClass(), GetMesh()->GetComponentLocation(), GetMesh()->GetComponentRotation(), SpawnActorOption);

	if(LeftHand)
	{
		LeftHand->AttachToComponent(GetMesh(), AttachRules);
	}

	RightHand = GetWorld()->SpawnActor<ARightHandMotionController>(RightHand->StaticClass(), GetMesh()->GetComponentLocation(), GetMesh()->GetComponentRotation(), SpawnActorOption);
	
	if (RightHand)
	{
		RightHand->AttachToComponent(GetMesh(), AttachRules);
	}

	if (HeadBox)
	{
		HeadBox->OnComponentBeginOverlap.AddDynamic(this, &AMotionControllerCharacter::OnHeadOverlap);		// 오버랩 이벤트를 발생시킬 수 있도록 설정
	}

	AttackPointSet();
}

// Called every frame
void AMotionControllerCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (CurrentHp > 100.0f)
	{
		CurrentHp = 100.0f;
	}
	if (CurrentStamina > 100.0f)
	{
		CurrentStamina = 100.0f;
	}

	if (DogArray.Num() > 0)
	{
		DogArray.Shrink();	// 메모리 최적화
	}

	/*ADog** Dog = DogArray.GetData();
	for (int i = 0; i <DogArray.Num(); i++)
	{
		UE_LOG(LogClass, Warning, TEXT("%d - %s"),i+1, *Dog[i]->GetName());
	}*/

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

	if (IsValid(Widget->GetRenderTarget()))
	{
		UTexture* texture;
		texture = Cast<UTextureRenderTarget2D>(Widget->GetRenderTarget());
		Stereo->SetTexture(Widget->GetRenderTarget());
	}

	//// Idle 상태, 움직이지 않을 때 카메라 숨쉬기 흔들림.
	//if (CurrentState != EPlayerState::Idle && this->GetVelocity().Size() == 0)
	//{
	//	CurrentState = EPlayerState::Idle;
	//	GetWorld()->GetTimerManager().SetTimer(SetIdleTimerHandle, this, &AMotionControllerCharacter::SetAllowBreathe, 0.01f, false, 1.0f);		// 1.5초 후 무적시간을 비활성화
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

	//  Test
	PlayerInputComponent->BindAction(TEXT("Menu"), IE_Released, this, &AMotionControllerCharacter::GameMenu);
}

// 오파시티값이 있어도 항상 그랩상태로 있는다고 가정할 때
void AMotionControllerCharacter::GrabLeftOn()
{

	// 왼손으로 할수 있는것은 아무것도 없으므로 LeftHand->GrabActor();를 빼는거 고려해야함, 그런데 문을 두손으로 연다고하면 조건문 줘서 문이 아닐때는 걸러줘야함

	GrabState = E_HandState::Grab;

	// GrabActor를 빼고 왼손은 문일 때를 가정해서 새로운 함수 만들기
	LeftHand->GrabActor();

	// if(잡힌게 문이면)
	// 문으로 작용하는 함수

	// 처음에 그랩상태를 State::Grab으로 세팅해야함, GrabActor()함수에서 반환값 bool로 변환해야함 -> 아직 확실하지 않으므로 장비로 테스트
	// if(잡힌게 없으면) 
	LeftHand->Shield->ConvertOfOpacity(1);
}

void AMotionControllerCharacter::GrabLeftOff()
{
	GrabState = E_HandState::Open;
	LeftHand->ReleaseActor();
	LeftHand->Shield->ConvertOfOpacity(0.5f);
}

void AMotionControllerCharacter::GrabRightOn()
{
	GrabState = E_HandState::Grab;

	RightHand->GrabActor();

	RightHand->Sword->ConvertOfOpacity(1);
}

void AMotionControllerCharacter::GrabRightOff()
{
	GrabState = E_HandState::Open;

	RightHand->ReleaseActor();

	RightHand->Sword->ConvertOfOpacity(0.5f);
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
	LaunchCharacter(DashVector, false, false);
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

void AMotionControllerCharacter::GameMenu()
{
	if (!Menu)
	{
		FActorSpawnParameters SpawnActorOption;
		SpawnActorOption.Owner = this;
		SpawnActorOption.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

		FVector location = FVector(Camera->GetComponentLocation().X, Camera->GetComponentLocation().Y, Camera->GetComponentLocation().Z);
		FRotator rotator = FRotator(0.0f, 180.0f, 0.0f);

		Menu = GetWorld()->SpawnActor<AMenu>(Menu->StaticClass(), location,
			rotator, SpawnActorOption);
	}
	else
	{
		Menu->Destroy();
		Menu = nullptr;
	}
	UE_LOG(LogTemp, Log, TEXT("Menu Tween"));
}

void AMotionControllerCharacter::AttackPointSet()
{
	AMyTargetPoint* AttackPoint;
	FAttachmentTransformRules AttachRules(EAttachmentRule::KeepWorld, EAttachmentRule::KeepWorld, EAttachmentRule::KeepWorld, false);
	FVector CalculatePoint;
	FVector InitPoint = Camera->GetComponentLocation();
	FVector Point;

	CalculatePoint = InitPoint;
	CalculatePoint.X = InitPoint.X + 200.0f;
	Point = CalculatePoint;
	Point.Z = Point.Z - 248.0f;

	AttackPoint = GetWorld()->SpawnActor<AMyTargetPoint>(AttackPoint->StaticClass(), Point, this->GetActorRotation());

	if (AttackPoint)
	{
		AttackPoints.Add(AttackPoint);
		AttackPoint->AttachToComponent(Camera, AttachRules);
	}	

	CalculatePoint.X = InitPoint.X - 200.0f;
	Point = CalculatePoint;
	Point.Z = Point.Z - 248.0f;

	AttackPoint = GetWorld()->SpawnActor<AMyTargetPoint>(AttackPoint->StaticClass(), Point, this->GetActorRotation());

	if (AttackPoint)
	{
		AttackPoints.Add(AttackPoint);
		AttackPoint->AttachToComponent(Camera, AttachRules);
	}	

	CalculatePoint = InitPoint;
	CalculatePoint.Y = InitPoint.Y - 200.0f;
	Point = CalculatePoint;
	Point.Z = Point.Z - 248.0f;

	AttackPoint = GetWorld()->SpawnActor<AMyTargetPoint>(AttackPoint->StaticClass(), Point, this->GetActorRotation());

	if (AttackPoint)
	{
		AttackPoints.Add(AttackPoint);
		AttackPoint->AttachToComponent(Camera, AttachRules);
	}

	CalculatePoint.Y = InitPoint.Y + 200.0f;
	Point = CalculatePoint;
	Point.Z = Point.Z - 248.0f;

	AttackPoint = GetWorld()->SpawnActor<AMyTargetPoint>(AttackPoint->StaticClass(), Point, this->GetActorRotation());

	if (AttackPoint)
	{
		AttackPoints.Add(AttackPoint);
		AttackPoint->AttachToComponent(Camera, AttachRules);
	}	

	CalculatePoint = InitPoint;
	CalculatePoint.X = InitPoint.X + 200.0f;
	CalculatePoint.Y = InitPoint.Y - 200.0f;
	Point = CalculatePoint;
	Point.Z = Point.Z - 248.0f;

	AttackPoint = GetWorld()->SpawnActor<AMyTargetPoint>(AttackPoint->StaticClass(), Point, this->GetActorRotation());

	if (AttackPoint)
	{
		AttackPoints.Add(AttackPoint);
		AttackPoint->AttachToComponent(Camera, AttachRules);
	}

	CalculatePoint = InitPoint;
	CalculatePoint.X = InitPoint.X + 200.0f;
	CalculatePoint.Y = InitPoint.Y + 200.0f;
	Point = CalculatePoint;
	Point.Z = Point.Z - 248.0f;

	AttackPoint = GetWorld()->SpawnActor<AMyTargetPoint>(AttackPoint->StaticClass(), Point, this->GetActorRotation());

	if (AttackPoint)
	{
		AttackPoints.Add(AttackPoint);
		AttackPoint->AttachToComponent(Camera, AttachRules);
	}

	CalculatePoint = InitPoint;
	CalculatePoint.X = InitPoint.X - 200.0f;
	CalculatePoint.Y = InitPoint.Y - 200.0f;
	Point = CalculatePoint;
	Point.Z = Point.Z - 248.0f;

	AttackPoint = GetWorld()->SpawnActor<AMyTargetPoint>(AttackPoint->StaticClass(), Point, this->GetActorRotation());

	if (AttackPoint)
	{
		AttackPoints.Add(AttackPoint);
		AttackPoint->AttachToComponent(Camera, AttachRules);
	}	

	CalculatePoint = InitPoint;
	CalculatePoint.X = InitPoint.X - 200.0f;
	CalculatePoint.Y = InitPoint.Y + 200.0f;
	Point = CalculatePoint;
	Point.Z = Point.Z - 248.0f;

	AttackPoint = GetWorld()->SpawnActor<AMyTargetPoint>(AttackPoint->StaticClass(), Point, this->GetActorRotation());

	if (AttackPoint)
	{
		AttackPoints.Add(AttackPoint);
		AttackPoint->AttachToComponent(Camera, AttachRules);
	}
}

void AMotionControllerCharacter::SetAllowBreathe()
{
	bAllowBreathe = true;			// 타이머로 움직인후 몇 초 후에 다시 숨쉴수 있게 함
}


float AMotionControllerCharacter::TakeDamage(float Damage, FDamageEvent const & DamageEvent, AController * EventInstigator, AActor * DamageCauser)
{

	if (!InvincibleTimeOn)
	{

		bisHit = true;
		Widget->bVisible = true;

		if (Widget->bVisible)		// 피격 위젯이 활성화 될때 실행
		{
			UHitBloodyWidget* bloodyWidget = Cast<UHitBloodyWidget>(Widget->GetUserWidgetObject());		// UHitBloodyWidget함수를 사용할수 있게 함
			if (bloodyWidget)
			{
				bloodyWidget->PlayAnimationByName("Bloody", 0.0, 1, EUMGSequencePlayMode::Forward, 1.0f);		// 애니메이션 실행
			}
		}

		CurrentHp -= Damage;			// 현재 체력감소
	
		//ULeftHandWidget* HandWidget = Cast<ULeftHandWidget>
		//	(LeftHand->Shield->CharacterStateWidget->GetUserWidgetObject());		// 왼손 방패의 위젯을 ULeftHandWidget내의 함수를 사용할 수 있도록 캐스트한다.

		//if (HandWidget)
		//{
		//	HandWidget->ReceiveDamage(Damage);			// 데미지를 받는다.
		//}

		LeftHand->Shield->StateBar->GetDamage(Damage);
		InvincibleTimeOn = true;		// 피격되면 즉시 무적시간 활성화
		GetWorld()->GetTimerManager().SetTimer(DamageTimerHandle, this, &AMotionControllerCharacter::DamageTimer, 0.01f, false, 1.5f);		// 1.5초 후 무적시간을 비활성화
	}

	return Damage;
}

void AMotionControllerCharacter::DamageTimer()
{
	InvincibleTimeOn = false;			// 무적시간 비활성화
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
		Widget->bVisible = false;		// 위젯을 보이지 않게 함
}

void AMotionControllerCharacter::OnHeadOverlap(UPrimitiveComponent * OverlappedComp, AActor * OtherActor, UPrimitiveComponent * OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult)
{
	if (OtherActor->ActorHasTag("Potion") && GrabState == E_HandState::Grab)		// 컴포넌트 기준, 액터 기준이면 첫번째 조건 OtherActor->ActorHasTas("Potion") 으로 변환
	{
		CurrentHp += 30;		// 회복량

		//ULeftHandWidget* HandWidget = Cast<ULeftHandWidget>
		//	(LeftHand->Shield->CharacterStateWidget->GetUserWidgetObject());		// 왼손 방패의 위젯을 ULeftHandWidget내의 함수를 사용할 수 있도록 캐스트한다.

		//if (HandWidget)
		//{
		//	HandWidget->GainHP(30);		// 회복
		//}
	}

	if (OtherComp->ComponentHasTag("DogAttackCollision"))
	{

		ARightHandMotionController* RightController = Cast<ARightHandMotionController>(RightHand);

		if (!RightController->AttachDog)
		{
			ADog* Dog = Cast<ADog>(OtherActor);
			if (Dog)
			{
				RightController->AttachDog = Dog;
				Dog->bIsAttack = true;
				Dog->DogAttackCollision->SetActive(false);

				// KeepRelative : 손의 각도가 같으면 붙는 각도도 일정함
				FAttachmentTransformRules AttachRules(EAttachmentRule::SnapToTarget, EAttachmentRule::SnapToTarget, EAttachmentRule::KeepWorld, true);

				Dog->AttachToComponent(RightController->AttachDogPosition, AttachRules);
				
				Dog->GetMesh()->SetAllBodiesBelowSimulatePhysics("Bip002-Neck", true, true);
				

				Dog->SetActorRelativeLocation(FVector(0.0f, 0.0f, 0.0f));
				Dog->SetActorRelativeRotation(FRotator(0.0f, 0.0f, 0.0f));

				Dog->CurrentDogState = EDogState::Bite;
				Dog->CurrentDogAnimState = EDogAnimState::Nothing;
				Dog->CurrentDogJumpState = EDogJumpState::Nothing;
				Dog->CurrentDogCircleState = EDogCircleState::Nothing;

				Dog->AttachActor = this;
				Dog->DogAttackCollision->bGenerateOverlapEvents = true;
			}
		}
	}
}
