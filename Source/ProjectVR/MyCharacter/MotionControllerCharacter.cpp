﻿// Fill out your copyright notice in the Description page of Project Settings.

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

#include "Monster/Dog/Dog.h"
#include "Components/CapsuleComponent.h"
#include "Components/SphereComponent.h"
#include "HeadMountedDisplayFunctionLibrary.h"
#include "MyCharacter/Widget/HPStaminaBar_2.h"
#include "MyCharacter/Widget/HPStaminaBar.h"

#include "MyCharacter/Widget/Menu.h"									// 메뉴의 활성화 / 비활성화
#include "Components/WidgetInteractionComponent.h"			// 위젯과의 상호작용
#include "TimerManager.h"			// 스테미너 자동 회복

#include "CameraLocation.h"
#include "Components/PawnNoiseEmitterComponent.h"

#include "Kismet/GameplayStatics.h"
#include "Engine/StaticMesh.h"
#include "Materials/MaterialInstanceDynamic.h"
#include "Level/MainMap/MainMapGameMode.h"
#include "Level/BossRoom/BossRoomGameMode.h"

// Sets default values
AMotionControllerCharacter::AMotionControllerCharacter()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	bUseControllerRotationYaw = true;

	GetCapsuleComponent()->SetCollisionProfileName(FName(TEXT("MyCharacter")));
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
	Widget->bVisible = false;

	HeadBox = CreateDefaultSubobject<UBoxComponent>(TEXT("HeadBox"));
	HeadBox->SetRelativeScale3D(FVector(0.5f, 0.5f, 0.5f));
	HeadBox->SetupAttachment(Camera);
	HeadBox->SetCollisionProfileName(TEXT("OverlapAll"));
	HeadBox->bGenerateOverlapEvents = true;
	HeadBox->ComponentTags.Add(FName("Head"));

	CameraLocation = nullptr;
	NoiseEmitter = CreateDefaultSubobject<UPawnNoiseEmitterComponent>(TEXT("NoiseEmitter"));

	GetCharacterMovement()->MaxWalkSpeed = 280.0f;

	DamagedBlood = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("DamagedBlood"));
	DamagedBlood->SetupAttachment(Camera);
	static ConstructorHelpers::FObjectFinder<UStaticMesh>SM_Blood(TEXT("StaticMesh'/Game/Assets/Effect/HitFeedback/blood.blood'"));
	if (SM_Blood.Succeeded())
	{
		DamagedBlood->SetStaticMesh(SM_Blood.Object);
	}
	DamagedBlood->SetRelativeScale3D(FVector(2.0f, 1.573f, 2.0f));
	DamagedBlood->SetCollisionProfileName("NoCollision");
	DamagedBlood->bVisible = false;

	MaxHp = 100.0f;
	CurrentHp = MaxHp;
	MaxStamina = 100.0f;
	CurrentStamina = MaxStamina;

	// 테스트 / 나중에 수정할 것
	AttackPoint = 5.0f;
	DefencePoint = 10.0f;
	DashPoint = 30.0f;
	RecoveryPoint = 1.0f;
	bIsUseStamina = false;

	DamagedValue = -1.0f;
	bisHit = false;
	bDash = false;
	bDeath = false;
	InvincibleTimeOn = false;
	CurrentState = EPlayerState::Idle;
	bAllowBreathe = true;
	DashPower = 800.0f;
	GrabState = E_HandState::Open;		// 나중에 무기 투명화 처리하면 그랩상태로 바꿔야함

	HeadBox->ComponentTags.Add(FName(TEXT("DisregardForLeftHand")));
	HeadBox->ComponentTags.Add(FName(TEXT("DisregardForRightHand")));
	Tags.Add(FName("Character"));
	Tags.Add(FName(TEXT("DisregardForLeftHand")));
	Tags.Add(FName(TEXT("DisregardForRightHand")));
}

// Called when the game starts or when spawned
void AMotionControllerCharacter::BeginPlay()
{
	Super::BeginPlay();

	DamagedMat_Inst = DamagedBlood->CreateDynamicMaterialInstance(0, DamagedBlood->GetMaterial(0));
	DamagedMat_Inst->SetScalarParameterValue("main", -1.0f);


	FName DeviceName = UHeadMountedDisplayFunctionLibrary::GetHMDDeviceName();

	if (DeviceName == "SteamVR" || DeviceName == "OculusHMD")
	{
		GLog->Log(FString::Printf(TEXT("DeviceName : %s"),*DeviceName.ToString()));
		UHeadMountedDisplayFunctionLibrary::SetTrackingOrigin(EHMDTrackingOrigin::Eye);
		if (SpringArm)
		{
			//SpringArm->SetRelativeLocation(FVector(0, 0, GetCapsuleComponent()->GetUnscaledCapsuleHalfHeight()*-1.0f));
		}
	}

	FActorSpawnParameters SpawnActorOption;
	SpawnActorOption.Owner = this;
	SpawnActorOption.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

	FAttachmentTransformRules AttachRules(EAttachmentRule::SnapToTarget, EAttachmentRule::SnapToTarget, EAttachmentRule::KeepWorld, false);

	LeftHand = GetWorld()->SpawnActor<ALeftHandMotionController>(LeftHand->StaticClass(), GetMesh()->GetComponentLocation(), GetMesh()->GetComponentRotation(), SpawnActorOption);

	if (LeftHand)
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

	/*CameraLocationChracter = GetWorld()->SpawnActor<ACameraLocationCharacter>(CameraLocationChracter->StaticClass());

	if (CameraLocationChracter)
	{
	CameraLocationChracter->AttachToComponent(Camera, AttachRules);
	}*/
	CameraLocation = GetWorld()->SpawnActor<ACameraLocation>(CameraLocation->StaticClass());

	if (CameraLocation)
	{
		CameraLocation->AttachToComponent(Camera, AttachRules);
	}

	GetWorld()->GetTimerManager().SetTimer(AutoTimerHandle, this, &AMotionControllerCharacter::AutoStamina, 0.05f, false);		// 자동으로 스테미너 채우기
	GrabLeftOn();
	GrabLeftOff();
}

// Called every frame
void AMotionControllerCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (GetVelocity().Size() > 100.0f)
	{
		MakeNoiseEmitter();
	}

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

	//if (DamagedBlood->bVisible)
	//{
	//	accTime += DeltaTime;
	//	if (accTime >= 2.0f)
	//	{
	//		accTime = 0.0f;
	//		GetWorld()->GetTimerManager().SetTimer(DamagedHandle, this, &AMotionControllerCharacter::FinishDamaged, 2.0f, false);
	//		/*DamagedMat_Inst->SetScalarParameterValue("main", DamagedValue);
	//		DamagedValue += 0.01f;
	//		if (DamagedValue >= 1.0f)
	//		{
	//			accTime = 0.0f;
	//			DamagedValue = -1.0f;
	//			DamagedBlood->bVisible = false;
	//		}*/
	//	}
	//}

	//UE_LOG(LogTemp, Log, TEXT("-- %f / %f / %f"), Camera->GetForwardVector().X, Camera->GetForwardVector().Y, Camera->GetForwardVector().Z);
	//UE_LOG(LogTemp, Log, TEXT("== %f / %f / %f"), Camera->GetUpVector().X, Camera->GetUpVector().Y, Camera->GetUpVector().Z);
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

	PlayerInputComponent->BindAction(TEXT("Dash"), IE_Pressed, this, &AMotionControllerCharacter::DashOn);
	PlayerInputComponent->BindAction(TEXT("Dash"), IE_Released, this, &AMotionControllerCharacter::DashOff);

	PlayerInputComponent->BindAxis(TEXT("MoveForward"), this, &AMotionControllerCharacter::MoveForward);
	PlayerInputComponent->BindAxis(TEXT("MoveRight"), this, &AMotionControllerCharacter::MoveRight);

	PlayerInputComponent->BindAction(TEXT("Run"), IE_Pressed, this, &AMotionControllerCharacter::RunOn);
	PlayerInputComponent->BindAction(TEXT("Run"), IE_Released, this, &AMotionControllerCharacter::RunOff);
	//  Test
	PlayerInputComponent->BindAction(TEXT("Menu"), IE_Released, this, &AMotionControllerCharacter::GameMenu);
}

// 오파시티값이 있어도 항상 그랩상태로 있는다고 가정할 때
void AMotionControllerCharacter::GrabLeftOn()
{
	// 왼손으로 할수 있는것은 아무것도 없으므로 LeftHand->GrabActor();를 빼는거 고려해야함, 그런데 문을 두손으로 연다고하면 조건문 줘서 문이 아닐때는 걸러줘야함

	LeftHand->interaction->PressPointerKey(EKeys::LeftMouseButton);

	GrabState = E_HandState::Grab;

	// GrabActor를 빼고 왼손은 문일 때를 가정해서 새로운 함수 만들기
	LeftHand->GrabActor();

	// if(잡힌게 문이면)
	// 문으로 작용하는 함수

	// 처음에 그랩상태를 State::Grab으로 세팅해야함, GrabActor()함수에서 반환값 bool로 변환해야함 -> 아직 확실하지 않으므로 장비로 테스트
	// if(잡힌게 없으면) 
	LeftHand->Shield->ConvertOfOpacity(0.14f);
}

void AMotionControllerCharacter::GrabLeftOff()
{
	LeftHand->interaction->ReleasePointerKey(EKeys::LeftMouseButton);

	GrabState = E_HandState::Open;
	LeftHand->ReleaseActor();
	LeftHand->Shield->ConvertOfOpacity(0.8f);
}

void AMotionControllerCharacter::GrabRightOn()
{
	RightHand->interaction->PressPointerKey(EKeys::LeftMouseButton);

	GrabState = E_HandState::Grab;

	RightHand->GrabActor();
}

void AMotionControllerCharacter::GrabRightOff()
{
	RightHand->interaction->ReleasePointerKey(EKeys::LeftMouseButton);

	GrabState = E_HandState::Open;

	RightHand->ReleaseActor();
}

// 플레이어의 상태에 따라 걷기 / 달리기 
void AMotionControllerCharacter::MoveForward(float Value)
{
	if (Value != 0)
	{
		if (CurrentState == EPlayerState::Run)
			GetCharacterMovement()->MaxWalkSpeed = RunSpeed;
		else
		{
			CurrentState = EPlayerState::Walk;
			GetCharacterMovement()->MaxWalkSpeed = WalkSpeed;
		}
		AddMovementInput(Camera->GetForwardVector(), Value);
	}
}

// 플레이어의 상태에 따라 걷기 / 달리기 
void AMotionControllerCharacter::MoveRight(float Value)
{
	if (Value != 0)
	{
		if (CurrentState == EPlayerState::Run)
			GetCharacterMovement()->MaxWalkSpeed = RunSpeed;
		else
		{
			CurrentState = EPlayerState::Walk;
			GetCharacterMovement()->MaxWalkSpeed = WalkSpeed;
		}
		AddMovementInput(Camera->GetRightVector(), Value);
	}
}

// 트랙패드를 누르면  Run상태
void AMotionControllerCharacter::RunOn()
{
	CurrentState = EPlayerState::Run;
}

// 트랙패드를 누르면 Run해제
void AMotionControllerCharacter::RunOff()
{
	CurrentState = EPlayerState::Idle;
}

void AMotionControllerCharacter::DashOn()
{
	if (GetVelocity().Size() >= 20.0f)
	{
		if (!bDash)
		{
			if (UseStamina(DashPoint))
			{
				FVector DashVector = FVector::ZeroVector;
				DashVector = GetVelocity().GetSafeNormal()*3000.0f;
				DashVector.Z = 20.0f;
				LaunchCharacter(DashVector, false, false);
				bDash = true;
			}
		}
	}
}

void AMotionControllerCharacter::DashOff()
{
	bDash = false;
}

void AMotionControllerCharacter::GameMenu()
{
	if (!Menu)
	{
		FActorSpawnParameters SpawnActorOption;
		SpawnActorOption.Owner = this;
		SpawnActorOption.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

		// 메뉴를 플레이어가 보는 위치와 알맞은 각도에 생성
		FVector location = FVector(Camera->GetComponentLocation().X, Camera->GetComponentLocation().Y, GetActorLocation().Z);
		FRotator rotator = FRotator(Camera->GetComponentRotation().Pitch + 20.0f, Camera->GetComponentRotation().Yaw + 180.0f, 0.0f);
		FVector CameraForwardVectorzeroHeight = FVector(Camera->GetForwardVector().X, Camera->GetForwardVector().Y, 0.0f);

		Menu = GetWorld()->SpawnActor<AMenu>(Menu->StaticClass(), location + CameraForwardVectorzeroHeight.GetSafeNormal() * 70.0f,
			rotator, SpawnActorOption);
	}
	else
	{
		Menu->Destroy();
		Menu = nullptr;
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
		GLog->Log(FString::Printf(TEXT("데미지 받음")));
		bisHit = true;
		//Widget->bVisible = true;
		DamagedMat_Inst->SetScalarParameterValue("main", -1.0f);
		DamagedBlood->bVisible = true;

		GetWorld()->GetTimerManager().SetTimer(DamagedHandle, this, &AMotionControllerCharacter::FinishDamaged, 0.1f, true,0.1f);

																															// 체력 감소
		if (CurrentHp > 0.0f)
		{
			CurrentHp -= Damage;
			if (CurrentHp < 0.0f)
				CurrentHp = 0.0f;
		}

		if (CurrentHp <= 0.0f && !bDeath)		// 피격 위젯이 활성화 될때 실행
		{
			bDeath = true;
			Widget->bVisible = true;
			UHitBloodyWidget* bloodyWidget = Cast<UHitBloodyWidget>(Widget->GetUserWidgetObject());		// UHitBloodyWidget함수를 사용할수 있게 함
			if (bloodyWidget)
			{
				bloodyWidget->PlayAnimationByName("Died", 0.0, 1, EUMGSequencePlayMode::Forward, 1.0f);		// 애니메이션 실행

				class APlayerController* MyPC = Cast<APlayerController>(GetController());
				MyPC->ClientSetCameraFade(true, FColor::Black, FVector2D(0.0, 1.0), 2.0);
				GetWorld()->GetTimerManager().SetTimer(MoveMainHandle, this, &AMotionControllerCharacter::MainScene, 2.5f, false);
			}
		}

		LeftHand->Shield->StateBar->GetDamage(Damage);
		InvincibleTimeOn = true;		// 피격되면 즉시 무적시간 활성화
		GetWorld()->GetTimerManager().SetTimer(DamageTimerHandle, this, &AMotionControllerCharacter::DamageTimer, 1.0f, false);		// 1.5초 후 무적시간을 비활성화
	}

	return Damage;
}

void AMotionControllerCharacter::DamageTimer()
{
	InvincibleTimeOn = false;			// 무적시간 비활성화
}

void AMotionControllerCharacter::MainScene()
{
	AMainMapGameMode* MainMapGM = Cast<AMainMapGameMode>(UGameplayStatics::GetGameMode(GetWorld()));

	if (MainMapGM)
	{
		UGameplayStatics::OpenLevel(GetWorld(), "Area4_Temple");
	}
	else
	{
		ABossRoomGameMode* BossGM = Cast<ABossRoomGameMode>(UGameplayStatics::GetGameMode(GetWorld()));

		if (BossGM)
		{
			UGameplayStatics::OpenLevel(GetWorld(), "Cathedral_inside");
		}
	}
	
}

void AMotionControllerCharacter::MoveMainScene()
{
	class APlayerController* MyPC = Cast<APlayerController>(GetController());
	MyPC->ClientSetCameraFade(true, FColor::Black, FVector2D(0.0, 1.0), 2.0);
	GetWorld()->GetTimerManager().SetTimer(MoveMainHandle, this, &AMotionControllerCharacter::MainScene, 2.5f, false);
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
		RightHand->HandFormState = EHandFormState::WeaponHandGrab;

		CurrentHp += 30;		// 회복량

	}
}

void AMotionControllerCharacter::MakeNoiseEmitter()
{
	NoiseEmitter->MakeNoise(this, 0.8f, GetActorLocation());
	NoiseEmitter->NoiseLifetime = 0.2f;
}

void AMotionControllerCharacter::FinishDamaged()
{	
	DamagedValue += 0.1f;
	DamagedMat_Inst->SetScalarParameterValue(TEXT("main"),DamagedValue);

	if (DamagedValue > 0.9f)
	{
		DamagedValue = -1.0f;
		DamagedBlood->bVisible = false;
		GetWorld()->GetTimerManager().ClearTimer(DamagedHandle);
	}	
}

bool AMotionControllerCharacter::UseStamina(float _stamina)
{
	if (CurrentStamina < _stamina)
	{
		return false;
	}
	CurrentStamina -= _stamina;
	bIsUseStamina = true;

	GetWorld()->GetTimerManager().ClearTimer(AutoTimerHandle);			// 스테미너 사용 동작은 잠시 스테미너 회복을 멈춤
	GetWorld()->GetTimerManager().SetTimer(AutoTimerHandle, this, &AMotionControllerCharacter::AutoStamina, 1.5f, false);		// 그리고 바로 3초후 예약(스테미너 자동 회복
	return true;
}

void AMotionControllerCharacter::AutoStamina()
{
	if (CurrentStamina < MaxStamina)			// 스테미너가 Full인 상태가 아닐 때만 실행
	{
		CurrentStamina += RecoveryPoint;			// 수치만큼 스테미너 증가

		if (CurrentStamina > MaxStamina)
		{
			// 다 차게 되면 스테미너를 Full과 맞춰줌
			CurrentStamina = MaxStamina;
		}
		GetWorld()->GetTimerManager().SetTimer(AutoTimerHandle, this, &AMotionControllerCharacter::AutoStamina, 0.01f, false);			// 계속 스테미너 증가
	}
}
