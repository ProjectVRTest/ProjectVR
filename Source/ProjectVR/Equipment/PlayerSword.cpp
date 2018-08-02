// Fill out your copyright notice in the Description page of Project Settings.

#include "PlayerSword.h"
#include "Components/StaticMeshComponent.h"			// ����ƽ�޽� ������Ʈ
#include "UObject/ConstructorHelpers.h"					// ConstructorHelpers ���
#include "Components/CapsuleComponent.h"				// ĸ��������Ʈ
#include "Kismet/GameplayStatics.h"							// ���������޽� ���

#include "MyCharacter/MotionControllerCharacter.h"
#include "HandMotionController/Widget/LeftHandWidget.h"
#include "HandMotionController/LeftHandMotionController.h"
#include "PlayerShield.h"

#include "Runtime/Engine/Classes/Materials/MaterialParameterCollectionInstance.h"
#include "Runtime/Engine/Classes/Materials/MaterialParameterCollection.h"
// Sets default values
APlayerSword::APlayerSword()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	/* ����ƽ �Ž� ������Ʈ ���� */
	SwordMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("SwordMesh"));
	SwordMesh->SetupAttachment(RootComponent);			// ����ƽ �޽� ������Ʈ�� ��Ʈ ������Ʈ�� �ǵ��� ��
	SwordMesh->SetCollisionProfileName(TEXT("NoCollision"));		// �޽��� �ݸ��� ���°��� NoCollision���� ��.

	static ConstructorHelpers::FObjectFinder<UStaticMesh>SM_Sword(TEXT("StaticMesh'/Game/Assets/Equipment/Mesh/sword_high.sword_high'"));		// ���۷��� ��η� ���� �Ž��� ã��
	if (SM_Sword.Succeeded())		// �� �޽��� ã���� ��� ����
	{
		SwordMesh->SetStaticMesh(SM_Sword.Object);			// ����ƽ �޽��� �� ��� ����
	}

	static ConstructorHelpers::FObjectFinder<UMaterialInterface> SwordMaterial(TEXT("Material'/Game/Assets/Equipment/Texture/SwordMaterial.SwordMaterial'"));
	if (SwordMaterial.Succeeded())
	{
		SwordMesh->SetMaterial(0, SwordMaterial.Object);
	}

	/* �ݸ��� ������Ʈ ���� */
	SwordCollision = CreateDefaultSubobject<UCapsuleComponent>(TEXT("SwordCollision"));			
	SwordCollision->SetupAttachment(SwordMesh);		// �ݸ����� �� �޽��� ����

	// �ݸ��� ��ġ �� ����, ũ�� ����
	SwordCollision->SetRelativeLocation(FVector(0.0f, 450.0f, 0.0f));
	SwordCollision->SetRelativeRotation(FRotator(0.0f,0.0f,90.0f));
	SwordCollision->SetRelativeScale3D(FVector(1.0f, 1.0f, 8.0f));

	// �� �޽��� ũ�� ����
	SwordMesh->SetRelativeScale3D(FVector(0.25f, 0.25f, 0.25f));


	Timer = 0.0f;		// Ÿ�̸� �ʱ�ȭ

	Tags.Add(FName(TEXT("PlayerSword")));		// ������ ���и� 'PlayerSword'�� �̸����� �±׸� ��
}

// Called when the game starts or when spawned
void APlayerSword::BeginPlay()
{
	Super::BeginPlay();
	
	static UMaterialParameterCollection* Collection = Cast<UMaterialParameterCollection>(StaticLoadObject(UMaterialParameterCollection::StaticClass(), NULL,		// ���׸��� �ݷ��� ã��
		TEXT("MaterialParameterCollection'/Game/Assets/Equipment/Texture/EquipmentMaterialCollection.EquipmentMaterialCollection'"), NULL, LOAD_None, NULL));

	if (Collection)
	{
		//UE_LOG(LogTemp, Warning, TEXT("dd %s"), *Collection->GetName());
		CollectionInstance = GetWorld()->GetParameterCollectionInstance(Collection);		// ã�� �ݷ����� �ݷ����ν��Ͻ��� ����
		CollectionInstance->SetScalarParameterValue(FName("Opacity_Sword"), 0.75);		// 'Opacity_Sword'���� ���� �Ķ���� ���� ����
	}

	if (SwordMesh)	
	{
		SwordMesh->OnComponentBeginOverlap.AddDynamic(this, &APlayerSword::OnSwordOverlap);		// ������ �̺�Ʈ�� �߻���ų �� �ֵ��� ����
	}
}

// Called every frame
void APlayerSword::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	Timer += DeltaTime;		// Ÿ�̸�
}

void APlayerSword::OnSwordOverlap(UPrimitiveComponent * OverlappedComp, AActor * OtherActor, UPrimitiveComponent * OtherComp,
	int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult)		// �������̺�Ʈ�� ������ �͵�
{
	if (OtherActor->ActorHasTag("Monster"))		// �������� ���Ͱ� 'Monster'��� �±׸� ������ ������ ����
	{
		UE_LOG(LogClass, Warning, TEXT("22222222222222222"));
		if (Timer >= 0.5f)			// Ÿ�̸Ӱ� 0.5 �̻��� ���� ������ ���� �� ���� (����1)
		{
			if (SwordMesh->GetPhysicsLinearVelocity().Size() >= 200.0f)		// ���ӵ��� ũ�Ⱑ 200 �̻��� ���� ���� ������ �Ͼ (����2)
			{
				Timer = 0.0f;		// ���� ������ �Ͼ�� �� Ÿ�̸� 0����

				if (SwordMesh->GetPhysicsLinearVelocity().Size() <= 500)		// ���ӵ��� ũ�Ⱑ 500������ �� ������ 10 (����4)
					Damage = 10.0f;
				else			// ���ӵ��� ũ�Ⱑ 500�ʰ��� �� ������ 15 (����4)
					Damage = 15.0f;

				//AMotionControllerCharacter* MyCharacter = Cast<AMotionControllerCharacter>(GetOwner());
				//ULeftHandWidget* MyStateUI = Cast<ULeftHandWidget>(MyCharacter->LeftHand->Shield->CharacterStateWidget);
				// ���� ĳ���� ���׹̳� ����
				// ���� UI���׹̳� ����
 				 
				UGameplayStatics::ApplyDamage(OtherActor, Damage, UGameplayStatics::GetPlayerController(GetWorld(), 0), this, nullptr);		// �������� ���Ϳ� ������ ����
				// (�������߻��� ����, ������, ������������ ��ü, ���� �������� ������ü, ����������Ŭ����)
			}
		}
	}	
}

void APlayerSword::ConvertOfOpacity(float opacity)		// Opacity�� ����(ĳ���Ϳ��� ȣ��)
{
	static UMaterialParameterCollection* Collection = Cast<UMaterialParameterCollection>(StaticLoadObject(UMaterialParameterCollection::StaticClass(), NULL,		// ���׸��� �ݷ��� ã��
		TEXT("MaterialParameterCollection'/Game/Assets/Equipment/Texture/EquipmentMaterialCollection.EquipmentMaterialCollection'"), NULL, LOAD_None, NULL));

	if (Collection)
	{
		//UE_LOG(LogTemp, Warning, TEXT("dd %s"), *Collection->GetName());
		CollectionInstance = GetWorld()->GetParameterCollectionInstance(Collection);		// ã�� �ݷ����� �ݷ����ν��Ͻ��� ����
		CollectionInstance->SetScalarParameterValue(FName("Opacity_Sword"), opacity);		// 'Opacity_Sword'���� ���� �Ķ���� ���� ����
	}
}
