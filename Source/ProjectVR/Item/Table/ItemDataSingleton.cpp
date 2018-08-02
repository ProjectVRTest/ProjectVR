// Fill out your copyright notice in the Description page of Project Settings.

#include "ItemDataSingleton.h"
#include "UObject/ConstructorHelpers.h"

AItemDataSingleton* AItemDataSingleton::instance = nullptr;

// Sets default values
AItemDataSingleton::AItemDataSingleton()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	//에디터 상에 있는 데이터테이블을 읽어서 DT_ItemDataTable에 저장한다.
	static ConstructorHelpers::FObjectFinder<UDataTable> DT_ItemDataTable(TEXT("DataTable'/Game/Blueprints/Item/ItemDataTable.ItemDataTable'"));

	if (DT_ItemDataTable.Succeeded()) //저장하는데 성공하면
	{
		//DataTable에 읽어들인 객체를 저장한다.
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

//외부로부터 인덱스 값을 받아서 데이터 테이블에서 해당하는 인덱스의 행값을 반환해주는 함수
FItemDataTable & AItemDataSingleton::GetItemData(int ItemIndex)
{
	return *DataTable->FindRow<FItemDataTable>(*FString::FromInt(ItemIndex), TEXT("ItemIndex"));
}

void AItemDataSingleton::Create()
{
	if (instance == nullptr)
	{
		instance = this; //인스턴스에 자기자신을 담아둔다.
	}
}

AItemDataSingleton * AItemDataSingleton::GetInstatnce()
{
	return instance; //자기자신을 반환한다.
}

void AItemDataSingleton::Destroy()
{
	if (instance != nullptr)
	{
		instance = nullptr;
	}
}

