// Fill out your copyright notice in the Description page of Project Settings.

#include "Potion.h"
#include "Components/StaticMeshComponent.h"			// ���� ����ƽ �޽�
#include "UObject/ConstructorHelpers.h"					// ������ �������κ��� ������ ����� ������ �� ���
#include "MyCharacter/MotionControllerCharacter.h"
#include "Item/Table/ItemDataSingleton.h"
#include "Engine/StreamableManager.h"
#include "Engine/StaticMesh.h"


// Sets default values
APotion::APotion()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("PotionMesh"));
	Mesh->SetupAttachment(RootComponent);

	Mesh->SetSimulatePhysics(false);
	Mesh->SetRelativeScale3D(FVector(0.1f, 0.1f, 0.1f));		// ũ�� ����		-> �׽�Ʈ�� ���Ǹ޽� ������ �׽�Ʈ�ؼ� �����
	Mesh->SetCollisionProfileName(TEXT("OverlapAll"));		// �׻� �������ǰ� ����


															// ���� �±�
	Tags.Add(FName(TEXT("Potion")));		// ������ ������ 'Potion'�� �̸����� �±׸� ��
}

// Called when the game starts or when spawned
void APotion::BeginPlay()
{
	Super::BeginPlay();

	Mesh->OnComponentBeginOverlap.AddDynamic(this, &APotion::OnPotionBeginOverlap);
	
	if (AItemDataSingleton::GetInstatnce()->DataTable) //�̱��� ��ü�� ������ ���̺��� �ִ��� �켱 Ȯ���Ѵ�.
	{
		//������ ù��°�� �ִ� �ε����� �ش��ϴ� ���������̺��� �ప�� �����ͼ� DataTable�� �����Ѵ�.
		DataTable = AItemDataSingleton::GetInstatnce()->GetItemData(1);

		//�޽��� �ε��ϱ� ���� ������ �����Ѵ�.
		FStreamableManager AssetLoader;

		//������ Mesh�� �տ��� �����ص� DataTable�� ItemMesh�� �̿��ؼ� �������ش�.
		Mesh->SetStaticMesh(AssetLoader.LoadSynchronous<UStaticMesh>(DataTable.ItemMesh));
	}
}

// Called every frame
void APotion::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void APotion::OnPotionBeginOverlap(UPrimitiveComponent * OverlappedComponent, AActor * OtherActor, UPrimitiveComponent * OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult)
{
	if (OtherActor->ActorHasTag(TEXT("Land")))
	{
		Destroy(this);
	}

	if (OtherComp->ComponentHasTag("Head"))
	{
		Destroy(this);
		TokenCompleteDelegate.ExecuteIfBound();
	}
}

