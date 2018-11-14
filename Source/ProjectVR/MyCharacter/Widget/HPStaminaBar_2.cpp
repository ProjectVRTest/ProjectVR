// Fill out your copyright notice in the Description page of Project Settings.

#include "HPStaminaBar_2.h"
#include "Components/StaticMeshComponent.h"			// 박스 메쉬
#include "UObject/ConstructorHelpers.h"					// 레퍼런스 경로
#include "Engine/StaticMesh.h"
#include "MyCharacter/MotionControllerCharacter.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
AHPStaminaBar_2::AHPStaminaBar_2()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Scene = CreateDefaultSubobject<USceneComponent>(TEXT("Scenes"));
	Scene->SetupAttachment(RootComponent);

	HPBackground = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("HPBackground"));
	HPBackground->SetupAttachment(Scene);

	HPScene = CreateDefaultSubobject<USceneComponent>(TEXT("HPScene"));
	HPScene->SetupAttachment(HPBackground);

	HPBar = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("HPBar"));
	HPBar->SetupAttachment(HPScene);
	static ConstructorHelpers::FObjectFinder<UStaticMesh> HPBar_SM(TEXT("StaticMesh'/Game/Assets/CharacterEquipment/StarterContent/Shapes/Shape_Cube.Shape_Cube'"));
	if (HPBar_SM.Succeeded())
	{
		HPBar->SetStaticMesh(HPBar_SM.Object);
		HPBackground->SetStaticMesh(HPBar_SM.Object);
	}
	static ConstructorHelpers::FObjectFinder<UMaterialInterface>HPBackground_Material(TEXT("Material'/Game/Assets/ui/StateBar/2-1_Mat.2-1_Mat'"));
	if (HPBackground_Material.Succeeded())
		HPBackground->SetMaterial(0, HPBackground_Material.Object);

	static ConstructorHelpers::FObjectFinder<UMaterialInterface>HP_Material(TEXT("MaterialInstanceConstant'/Game/Assets/ui/StateBar/M_HP.M_HP'"));
	if (HP_Material.Succeeded())
		HPBar->SetMaterial(0, HP_Material.Object);



	StaminaBackground = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaminaBackground"));
	StaminaBackground->SetupAttachment(Scene);

	SteminaScene = CreateDefaultSubobject<USceneComponent>(TEXT("SteminaScene"));
	SteminaScene->SetupAttachment(StaminaBackground);

	StaminaBar = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaminaBar"));
	StaminaBar->SetupAttachment(SteminaScene);
	static ConstructorHelpers::FObjectFinder<UStaticMesh> StaminaBar_SM(TEXT("StaticMesh'/Game/Assets/CharacterEquipment/StarterContent/Shapes/Shape_Cube.Shape_Cube'"));
	if (StaminaBar_SM.Succeeded())
	{
		StaminaBar->SetStaticMesh(StaminaBar_SM.Object);
		StaminaBackground->SetStaticMesh(StaminaBar_SM.Object);
	}
	static ConstructorHelpers::FObjectFinder<UMaterialInterface>StaminaBackground_Material(TEXT("Material'/Game/Assets/ui/StateBar/3-1_Mat.3-1_Mat'"));
	if (StaminaBackground_Material.Succeeded())
		StaminaBackground->SetMaterial(0, StaminaBackground_Material.Object);

	static ConstructorHelpers::FObjectFinder<UMaterialInterface>Stamina_Material(TEXT("MaterialInstanceConstant'/Game/Assets/ui/StateBar/M_Stamina.M_Stamina'"));
	if (Stamina_Material.Succeeded())
		StaminaBar->SetMaterial(0, Stamina_Material.Object);

	/*StaminaBackground = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaminaBackground"));
	StaminaBackground->SetupAttachment(Scene);

	SteminaScene = CreateDefaultSubobject<USceneComponent>(TEXT("SteminaScene"));
	SteminaScene->SetupAttachment(StaminaBackground);

	StaminaBar = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("SteminaBar"));
	StaminaBar->SetupAttachment(SteminaScene);
	static ConstructorHelpers::FObjectFinder<UStaticMesh> StaminaBar_SM(TEXT("StaticMesh'/Game/Assets/CharacterEquipment/StarterContent/Shapes/Shape_Cube.Shape_Cube'"));
	if (StaminaBar_SM.Succeeded())
	{
		StaminaBar->SetStaticMesh(StaminaBar_SM.Object);
		StaminaBackground->SetStaticMesh(StaminaBar_SM.Object);
	}
	static ConstructorHelpers::FObjectFinder<UMaterialInterface>StaminaBackground_Material(TEXT("Material'/Game/Assets/ui/StateBar/3-1_Mat.3-1_Mat'"));
	if (StaminaBackground_Material.Succeeded())
		StaminaBackground->SetMaterial(0, StaminaBackground_Material.Object);

	static ConstructorHelpers::FObjectFinder<UMaterialInterface>Stamina_Material(TEXT("Material'/Game/Assets/ui/StateBar/3_Mat.3_Mat'"));
	if (Stamina_Material.Succeeded())
		StaminaBar->SetMaterial(0, Stamina_Material.Object);*/


	// 사이즈 및 위치 지정
	HPBackground->SetRelativeLocation(FVector(0.0f, 0.0f, 0.0f));
	HPBackground->SetRelativeScale3D(FVector(0.3f, -0.001f, 0.02f));
	HPScene->SetRelativeLocation(FVector(-50.0f, -60.442318f, 0.0f));
	HPScene->SetRelativeScale3D(FVector(1.0f, 1.0f, 1.0f));
	HPBar->SetRelativeLocation(FVector(50.001f, 83.0f, 0.0f));
	HPBar->SetRelativeScale3D(FVector(1.0f, 0.557854f, 0.968437f));

	StaminaBackground->SetRelativeLocation(FVector(0.0f, 0.0f, -2.5f));
	StaminaBackground->SetRelativeScale3D(FVector(0.3f, -0.001f, 0.007f));
	SteminaScene->SetRelativeLocation(FVector(-50.0f, -60.442318f, 0.0f));
	SteminaScene->SetRelativeScale3D(FVector(1.0f, 1.0f, 1.0f));
	StaminaBar->SetRelativeLocation(FVector(50.001f, 83.0f, 0.0f));
	StaminaBar->SetRelativeScale3D(FVector(1.0f, 0.557854f, 0.968437f));

	/*StaminaBackground->SetRelativeLocation(FVector(0.0f, 0.0f, -1.558897f));
	StaminaBackground->SetRelativeScale3D(FVector(0.3f, -0.001f, 0.007f));
	SteminaScene->SetRelativeLocation(FVector(-50.0f, 0.0f, 4.0f));
	SteminaScene->SetRelativeScale3D(FVector(1.0f, 1.0f, 1.0f));
	StaminaBar->SetRelativeLocation(FVector(50.001f, 50.0f, 0.000076f));
	StaminaBar->SetRelativeScale3D(FVector(1.0f, 0.007f, 1.0f));*/

	// 오버랩 이벤트 설정
	HPBackground->SetCollisionProfileName("NoCollision");
	HPBar->SetCollisionProfileName("NoCollision");
	HPBackground->SetEnableGravity(false);
	HPBar->SetEnableGravity(false);
	HPBackground->bGenerateOverlapEvents = false;
	HPBar->bGenerateOverlapEvents = false;

	StaminaBackground->SetCollisionProfileName("NoCollision");
	StaminaBar->SetCollisionProfileName("NoCollision");
	StaminaBackground->SetEnableGravity(false);
	StaminaBar->SetEnableGravity(false);
	StaminaBackground->bGenerateOverlapEvents = false;
	StaminaBar->bGenerateOverlapEvents = false;
	/*StaminaBackground->SetCollisionProfileName("NoCollision");
	StaminaBar->SetCollisionProfileName("NoCollision");
	StaminaBackground->SetEnableGravity(false);
	StaminaBar->SetEnableGravity(false);
	StaminaBackground->bGenerateOverlapEvents = false;
	StaminaBar->bGenerateOverlapEvents = false;*/

	Tags.Add(FName(TEXT("StateBar")));
	Tags.Add(FName(TEXT("DisregardForLeftHand")));
	Tags.Add(FName(TEXT("DisregardForRightHand")));
}

// Called when the game starts or when spawned
void AHPStaminaBar_2::BeginPlay()
{
	Super::BeginPlay();

	// 초기값 설정
	OwnerCharacter = Cast<AMotionControllerCharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));
	if (OwnerCharacter)
	{
		MaxHP = OwnerCharacter->MaxHp;
		CurrentHP = OwnerCharacter->CurrentHp / MaxHP;
		VirtualHP = CurrentHP;

		MaxStamina = OwnerCharacter->MaxStamina;
		CurrentStamina = OwnerCharacter->CurrentStamina / MaxStamina;
		VirtualStamina = CurrentStamina;
		UE_LOG(LogTemp, Log, TEXT("%f / %f / %f"), CurrentStamina, MaxStamina, VirtualStamina);
	}
}

// Called every frame
void AHPStaminaBar_2::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (OwnerCharacter)
	{
		VirtualHP = OwnerCharacter->CurrentHp / MaxHP;

		VirtualStamina = OwnerCharacter->CurrentStamina / MaxStamina;
	}

	if (CurrentHP != VirtualHP)
	{
		ChangOfHP = DeltaTime * 1.0f;		// 변화량
											// 데미지 받음
		if (CurrentHP > VirtualHP)
		{
			CurrentHP -= ChangOfHP;
			HPScene->SetRelativeScale3D(FVector(CurrentHP, 1.0f, 1.0f));
			if (CurrentHP < VirtualHP)					// 현재HP와 가상HP를 맞추기
				CurrentHP = VirtualHP;
		}
		// HP 회복
		else
		{
			CurrentHP += ChangOfHP;
			HPScene->SetRelativeScale3D(FVector(CurrentHP, 1.0f, 1.0f));
			if (CurrentHP > VirtualHP)					// 현재HP와 가상HP를 맞추기
				CurrentHP = VirtualHP;
		}
	}

	if (CurrentStamina != VirtualStamina)
	{
		//UE_LOG(LogTemp, Log, TEXT("%f / %f"), CurrentStamina, VirtualStamina);
		ChangOfStamina = DeltaTime * 1.0f;		// 변화량

												// 데미지 받음
		if (CurrentStamina > VirtualStamina)
		{
			CurrentStamina -= ChangOfStamina;
			SteminaScene->SetRelativeScale3D(FVector(CurrentStamina, 1.0f, 1.0f));
			if (CurrentStamina < VirtualStamina)					// 현재HP와 가상HP를 맞추기
				CurrentStamina = VirtualStamina;
		}
		// HP 회복
		else
		{
			CurrentStamina += ChangOfStamina;
			SteminaScene->SetRelativeScale3D(FVector(CurrentStamina, 1.0f, 1.0f));
			if (CurrentStamina > VirtualStamina)					// 현재HP와 가상HP를 맞추기
				CurrentStamina = VirtualStamina;
		}
	}
}

void AHPStaminaBar_2::GetDamage(float damage)
{
	VirtualHP -= damage / 100;			// 데미지를 얻어 가상체력을 감소시킨다.
}

void AHPStaminaBar_2::RecoveryHP(float recoveryHP)
{
	VirtualHP += recoveryHP / 100;

	if (VirtualHP >= 1.0f)
		VirtualHP = 1.0f;
}

void AHPStaminaBar_2::UseStamina(float _useValue)
{
	VirtualStamina -= _useValue / 100;
}

void AHPStaminaBar_2::RecoveryStamina(float _useValue)
{
}

void AHPStaminaBar_2::AutoGainStamina()
{
}
