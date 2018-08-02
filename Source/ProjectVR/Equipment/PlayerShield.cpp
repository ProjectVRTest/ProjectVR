// Fill out your copyright notice in the Description page of Project Settings.

#include "PlayerShield.h"
#include "Components/WidgetComponent.h"			// ���� ������Ʈ
#include "Components/StaticMeshComponent.h"		// ����ƽ �޽� ������Ʈ
#include "UObject/ConstructorHelpers.h"				// ConstructorHelpers ���

#include "Runtime/Engine/Classes/Materials/MaterialParameterCollectionInstance.h"
#include "Runtime/Engine/Classes/Materials/MaterialParameterCollection.h"

// Sets default values
APlayerShield::APlayerShield()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	/* �� ������Ʈ ���� */
	ShieldScene = CreateDefaultSubobject<USceneComponent>(TEXT("ShieldScene"));
	ShieldScene->SetupAttachment(RootComponent);		// �� ������Ʈ�� ��Ʈ ������Ʈ�� �ǵ��� ��

	/* ����ƽ �Ž� ������Ʈ ���� */
	ShieldMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ShieldMesh"));

	ShieldMesh->SetCollisionProfileName(TEXT("OverlapOnlyPawn"));			// ĳ���Ϳ��� �浹�� ���ϱ����ؼ� Pawn(ĳ����)�� Overlap�ǵ��� ����
	ShieldMesh->SetupAttachment(ShieldScene);		// ������ ����ƽ �Ž��� �� ������Ʈ�� ����

	static ConstructorHelpers::FObjectFinder<UStaticMesh>SM_Shield(TEXT("StaticMesh'/Game/Assets/Equipment/Mesh/shield_low.shield_low'"));		// ���۷��� ��η� ���� �Ž��� ã��
	if (SM_Shield.Succeeded())		// ���� �޽��� ã���� ��� ����
	{
		ShieldMesh->SetStaticMesh(SM_Shield.Object);		// ����ƽ �޽��� ���� ��� ����
	}

	static ConstructorHelpers::FObjectFinder<UMaterialInterface> ShieldMaterial(TEXT("Material'/Game/Assets/Equipment/Texture/ShieldMaterial.ShieldMaterial'"));
	if (ShieldMaterial.Succeeded())
	{
		ShieldMesh->SetMaterial(0, ShieldMaterial.Object);
	}

	/* ���� ������Ʈ ���� */
	CharacterStateWidget = CreateDefaultSubobject<UWidgetComponent>(TEXT("StateInfoWidget"));	
	CharacterStateWidget->SetupAttachment(ShieldMesh);			// ������ ���� �޽��� ����

	static ConstructorHelpers::FClassFinder<UUserWidget> StaminaUI(TEXT("WidgetBlueprint'/Game/Blueprints/UI/LeftHandWidget.LeftHandWidget_C'"));		// ���۷��� ��η� ���� �������Ʈ�� ã��
	if (StaminaUI.Succeeded())		// �����������Ʈ�� ã���� ��� ����
	{
		CharacterStateWidget->SetWidgetClass(StaminaUI.Class);		// ã�� ���� Ŭ������ ����
	}

	// ������ ������ ��ġ�� ����, ũ��� ����
	CharacterStateWidget->SetRelativeRotation(FRotator(0, -90.0f, 0));
	CharacterStateWidget->SetRelativeScale3D(FVector(0.112368, -0.112368, 0.112368));
	CharacterStateWidget->SetRelativeLocation(FVector(9.0f, -16.0f, 60.0f));

	Tags.Add(FName(TEXT("PlayerShield")));		// ������ ���и� 'PlayerShield'�� �̸����� �±׸� ��
}

// Called when the game starts or when spawned
void APlayerShield::BeginPlay()
{
	Super::BeginPlay();
	
	static UMaterialParameterCollection* Collection = Cast<UMaterialParameterCollection>(StaticLoadObject(UMaterialParameterCollection::StaticClass(), NULL,		// ���׸��� �ݷ��� ã��
		TEXT("Material'/Game/Assets/Equipment/Texture/SwordMaterial.SwordMaterial'"), NULL, LOAD_None, NULL));

	if (Collection)
	{
		//UE_LOG(LogTemp, Warning, TEXT("dd %s"), *Collection->GetName());
		CollectionInstance = GetWorld()->GetParameterCollectionInstance(Collection);		// ã�� �ݷ����� �ݷ����ν��Ͻ��� ����
		CollectionInstance->SetScalarParameterValue(FName("Opacity_Shield"), 0.75f);		// 'Opacity_Sword'���� ���� �Ķ���� ���� ����
	}

}

// Called every frame
void APlayerShield::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void APlayerShield::ConvertOfOpacity(float opacity)		// Opacity�� ����(ĳ���Ϳ��� ȣ��)
{
	static UMaterialParameterCollection* Collection = Cast<UMaterialParameterCollection>(StaticLoadObject(UMaterialParameterCollection::StaticClass(), NULL,		// ���׸��� �ݷ��� ã��
		TEXT("Material'/Game/Assets/Equipment/Texture/SwordMaterial.SwordMaterial'"), NULL, LOAD_None, NULL));	

	if (Collection)
	{
		//UE_LOG(LogTemp, Warning, TEXT("dd %s"), *Collection->GetName());
		CollectionInstance = GetWorld()->GetParameterCollectionInstance(Collection);		// ã�� �ݷ����� �ݷ����ν��Ͻ��� ����
		CollectionInstance->SetScalarParameterValue(FName("Opacity_Shield"), opacity);		// 'Opacity_Sword'���� ���� �Ķ���� ���� ����
	}
}

