// Fill out your copyright notice in the Description page of Project Settings.

#include "ItemDataSingleton.h"
#include "UObject/ConstructorHelpers.h"

AItemDataSingleton* AItemDataSingleton::instance = nullptr;

// Sets default values
AItemDataSingleton::AItemDataSingleton()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	//������ �� �ִ� ���������̺��� �о DT_ItemDataTable�� �����Ѵ�.
	static ConstructorHelpers::FObjectFinder<UDataTable> DT_ItemDataTable(TEXT("DataTable'/Game/Blueprints/Item/ItemDataTable.ItemDataTable'"));

	if (DT_ItemDataTable.Succeeded()) //�����ϴµ� �����ϸ�
	{
		//DataTable�� �о���� ��ü�� �����Ѵ�.
		DataTable = DT_ItemDataTable.Object;
	}
}

// Called when the game starts or when spawned
void AItemDataSingleton::BeginPlay()
{
	Super::BeginPlay();

	Create();
}

// Called every frame
void AItemDataSingleton::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

//�ܺηκ��� �ε��� ���� �޾Ƽ� ������ ���̺��� �ش��ϴ� �ε����� �ప�� ��ȯ���ִ� �Լ�
FItemDataTable & AItemDataSingleton::GetItemData(int ItemIndex)
{
	return *DataTable->FindRow<FItemDataTable>(*FString::FromInt(ItemIndex), TEXT("ItemIndex"));
}

void AItemDataSingleton::Create()
{
	if (instance == nullptr)
	{
		instance = this; //�ν��Ͻ��� �ڱ��ڽ��� ��Ƶд�.
	}
}

AItemDataSingleton * AItemDataSingleton::GetInstatnce()
{
	return instance; //�ڱ��ڽ��� ��ȯ�Ѵ�.
}

void AItemDataSingleton::Destroy()
{
	if (instance != nullptr)
	{
		instance = nullptr;
	}
}

