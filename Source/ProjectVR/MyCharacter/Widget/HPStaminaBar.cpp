// Fill out your copyright notice in the Description page of Project Settings.

#include "HPStaminaBar.h"
#include "Components/StaticMeshComponent.h"			// �ڽ� �޽�
#include "UObject/ConstructorHelpers.h"					// ���۷��� ���
#include "Engine/StaticMesh.h"
#include "MyCharacter/MotionControllerCharacter.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
AHPStaminaBar::AHPStaminaBar()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Scene = CreateDefaultSubobject<USceneComponent>(TEXT("Scenes"));
	Scene->SetupAttachment(RootComponent);

	// HP
	HPScene = CreateDefaultSubobject<USceneComponent>(TEXT("HPScene"));
	HPScene->SetupAttachment(Scene);
	// HP
	HPBar = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("HPBar"));
	HPBar->SetupAttachment(HPScene);
	static ConstructorHelpers::FObjectFinder<UStaticMesh> HPBar_SM(TEXT("StaticMesh'/Game/Assets/CharacterEquipment/StarterContent/Shapes/Shape_Cube.Shape_Cube'"));
	if (HPBar_SM.Succeeded())
	{
		HPBar->SetStaticMesh(HPBar_SM.Object);
	}

	// Stamina
	SteminaScene = CreateDefaultSubobject<USceneComponent>(TEXT("SteminaScene"));
	SteminaScene->SetupAttachment(Scene);
	// Stamina
	StaminaBar = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("SteminaBar"));
	StaminaBar->SetupAttachment(SteminaScene);
	static ConstructorHelpers::FObjectFinder<UStaticMesh> StaminaBar_SM(TEXT("StaticMesh'/Game/Assets/CharacterEquipment/StarterContent/Shapes/Shape_Cube.Shape_Cube'"));
	if (HPBar_SM.Succeeded())
	{
		StaminaBar->SetStaticMesh(StaminaBar_SM.Object);
	}

	static ConstructorHelpers::FObjectFinder<UMaterialInterface>HP_Material(TEXT("Material'/Game/Blueprints/UI/HPTestMaterial.HPTestMaterial'"));
	if (HP_Material.Succeeded())
	{
		HPBar->SetMaterial(0, HP_Material.Object);
	}

	static ConstructorHelpers::FObjectFinder<UMaterialInterface>Stamina_Material(TEXT("Material'/Game/Blueprints/UI/StaminaTestMaterial.StaminaTestMaterial'"));
	if (Stamina_Material.Succeeded())
	{
		StaminaBar->SetMaterial(0, Stamina_Material.Object);
	}

	// �� ������ ����
	HPBarXSize = 0.3f;
	HPBarZSize = 0.02f;
	SteminaBarXSize = 0.3f;
	SteminaBarZSize = 0.02f;

	// ������ �� ��ġ ����
	HPScene->SetRelativeLocation(FVector(-(HPBarXSize / 2.0f)*100.0f, 0.0f, 0.0f));
	HPScene->SetRelativeScale3D(FVector(1.0f, 1.0f, 1.0f));
	SteminaScene->SetRelativeLocation(FVector(-(HPBarXSize / 2.0f)*100.0f, 0.0f, -(HPBarZSize * 200)));
	SteminaScene->SetRelativeScale3D(FVector(1.0f, 1.0f, 1.0f));

	HPBar->SetRelativeScale3D(FVector(HPBarXSize, 0.007f, HPBarZSize));
	HPBar->SetRelativeLocation(FVector((HPBarXSize / 2.0f)*100.0f, 0.0f, 0.0f));
	StaminaBar->SetRelativeScale3D(FVector(SteminaBarXSize, 0.007f, SteminaBarZSize));
	StaminaBar->SetRelativeLocation(FVector((HPBarXSize / 2.0f)*100.0f, 0.0f, 0.0f));

	// ������ �̺�Ʈ ����
	HPBar->SetCollisionProfileName("OverlapAll");
	HPBar->bGenerateOverlapEvents = false;
	StaminaBar->SetCollisionProfileName("OverlapAll");
	StaminaBar->bGenerateOverlapEvents = false;

	

	Tags.Add(FName(TEXT("StateBar"))); 
	Tags.Add(FName(TEXT("DisregardForLeftHand")));
	Tags.Add(FName(TEXT("DisregardForRightHand")));
}

// Called when the game starts or when spawned
void AHPStaminaBar::BeginPlay()
{
	Super::BeginPlay();
	
	// �ʱⰪ ����
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
void AHPStaminaBar::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (OwnerCharacter)
	{
		VirtualHP = OwnerCharacter->CurrentHp / MaxHP;

		VirtualStamina = OwnerCharacter->CurrentStamina / MaxStamina;
	}

	if (CurrentHP != VirtualHP)
	{
		ChangOfHP = DeltaTime * 0.5f;		// ��ȭ��
		// ������ ����
		if (CurrentHP > VirtualHP)
		{
			CurrentHP -= ChangOfHP;
			HPScene->SetRelativeScale3D(FVector(CurrentHP, 1.0f, 1.0f));
			if (CurrentHP < VirtualHP)					// ����HP�� ����HP�� ���߱�
				CurrentHP = VirtualHP;
		}
		// HP ȸ��
		else		
		{
			CurrentHP += ChangOfHP;
			HPScene->SetRelativeScale3D(FVector(CurrentHP, 1.0f, 1.0f));
			if (CurrentHP > VirtualHP)					// ����HP�� ����HP�� ���߱�
				CurrentHP = VirtualHP;
		}
	}
	
	if (CurrentStamina != VirtualStamina)
	{
		//UE_LOG(LogTemp, Log, TEXT("%f / %f"), CurrentStamina, VirtualStamina);
		ChangOfStamina = DeltaTime * 0.5f;		// ��ȭ��

		// ������ ����
		if (CurrentStamina > VirtualStamina)
		{
			CurrentStamina -= ChangOfStamina;
			SteminaScene->SetRelativeScale3D(FVector(CurrentStamina, 1.0f, 1.0f));
			if (CurrentStamina < VirtualStamina)					// ����HP�� ����HP�� ���߱�
				CurrentStamina = VirtualStamina;
		}
		// HP ȸ��
		else
		{
			CurrentStamina += ChangOfStamina;
			SteminaScene->SetRelativeScale3D(FVector(CurrentStamina, 1.0f, 1.0f));
			if (CurrentStamina > VirtualStamina)					// ����HP�� ����HP�� ���߱�
				CurrentStamina = VirtualStamina;
		}
	}
}

void AHPStaminaBar::GetDamage(float damage)
{
	VirtualHP -= damage / 100;			// �������� ��� ����ü���� ���ҽ�Ų��.
}

void AHPStaminaBar::RecoveryHP(float recoveryHP)
{
	VirtualHP += recoveryHP / 100;

	if (VirtualHP >= 1.0f)
		VirtualHP = 1.0f;
}

void AHPStaminaBar::UseStamina(float _useValue)
{
	VirtualStamina -= _useValue / 100;
}

void AHPStaminaBar::RecoveryStamina(float _useValue)
{
}

void AHPStaminaBar::AutoGainStamina()
{
}

