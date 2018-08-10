// Fill out your copyright notice in the Description page of Project Settings.

#include "Potion.h"
#include "Components/StaticMeshComponent.h"			// 포션 스태틱 메쉬
#include "UObject/ConstructorHelpers.h"					// 콘텐츠 브라우저로부터 포션의 모양을 가져올 때 사용
#include "MyCharacter/MotionControllerCharacter.h"
#include "Item/Table/ItemDataSingleton.h"
#include "Engine/StreamableManager.h"
#include "Engine/StaticMesh.h"

#include "kismet/GameplayStatics.h"
#include "MyCharacter/MotionControllerPC.h"
#include "HandMotionController/LeftHandMotionController.h"
#include "Item/PotionBag.h"

// Sets default values
APotion::APotion()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("PotionMesh"));
	SetRootComponent(Mesh);	

	BagInputFlag = false;
	BagInputCompleteFlag = false;

	//Mesh->SetRelativeScale3D(FVector(0.1f, 0.1f, 0.1f));		// 크기 설정		-> 테스트용 포션메쉬 나오면 테스트해서 재수정
	Mesh->SetCollisionProfileName(TEXT("NoCollision"));	
	Tags.Add(FName(TEXT("Potion")));		// 생성한 포션을 'Potion'란 이름으로 태그를 줌
	//Tags.Add(FName(TEXT("DisregardForLeftHand")));
}

// Called when the game starts or when spawned
void APotion::BeginPlay()
{
	Super::BeginPlay();

	Mesh->OnComponentBeginOverlap.AddDynamic(this, &APotion::OnPotionBeginOverlap);
	
	if (AItemDataSingleton::GetInstatnce()->DataTable) //싱글톤 객체에 데이터 테이블이 있는지 우선 확인한다.
	{
		//있으면 첫번째에 있는 인덱스에 해당하는 데이터테이블의 행값을 가져와서 DataTable에 저장한다.
		DataTable = AItemDataSingleton::GetInstatnce()->GetItemData(1);

		//메쉬를 로딩하기 위해 변수를 선언한다.
		FStreamableManager AssetLoader;

		//물약의 Mesh를 앞에서 저장해둔 DataTable의 ItemMesh를 이용해서 설정해준다.
		Mesh->SetStaticMesh(AssetLoader.LoadSynchronous<UStaticMesh>(DataTable.ItemMesh));
	}
}

// Called every frame
void APotion::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (BagInputCompleteFlag)
	{
		Destroy(this);
	}
}

void APotion::OnPotionBeginOverlap(UPrimitiveComponent * OverlappedComponent, AActor * OtherActor, UPrimitiveComponent * OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult)
{
	AMotionControllerPC* PC = Cast<AMotionControllerPC>(UGameplayStatics::GetPlayerController(GetWorld(), 0));
	
	if (PC)
	{
		AMotionControllerCharacter* MyCharacter = Cast<AMotionControllerCharacter>(PC->GetPawn());
		if (MyCharacter)
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
	}		
}

