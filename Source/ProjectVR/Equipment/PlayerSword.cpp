// Fill out your copyright notice in the Description page of Project Settings.

#include "PlayerSword.h"
#include "Components/StaticMeshComponent.h"			// 스태틱메쉬 컴포넌트
#include "UObject/ConstructorHelpers.h"					// ConstructorHelpers 사용
#include "Components/CapsuleComponent.h"				// 캡슐컴포넌트
#include "Kismet/GameplayStatics.h"							// 데미지전달시 사용

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

	/* 스태틱 매쉬 컴포넌트 생성 */
	SwordMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("SwordMesh"));
	SwordMesh->SetupAttachment(RootComponent);			// 스태틱 메쉬 컴포넌트가 루트 컴포넌트가 되도록 함
	SwordMesh->SetCollisionProfileName(TEXT("NoCollision"));		// 메쉬의 콜리전 상태값을 NoCollision으로 줌.

	static ConstructorHelpers::FObjectFinder<UStaticMesh>SM_Sword(TEXT("StaticMesh'/Game/Assets/Equipment/Mesh/sword_high.sword_high'"));		// 레퍼런스 경로로 방패 매쉬를 찾음
	if (SM_Sword.Succeeded())		// 검 메쉬를 찾았을 경우 실행
	{
		SwordMesh->SetStaticMesh(SM_Sword.Object);			// 스태틱 메쉬에 검 모양 설정
	}

	static ConstructorHelpers::FObjectFinder<UMaterialInterface> SwordMaterial(TEXT("Material'/Game/Assets/Equipment/Texture/SwordMaterial.SwordMaterial'"));
	if (SwordMaterial.Succeeded())
	{
		SwordMesh->SetMaterial(0, SwordMaterial.Object);
	}

	/* 콜리전 컴포넌트 생성 */
	SwordCollision = CreateDefaultSubobject<UCapsuleComponent>(TEXT("SwordCollision"));			
	SwordCollision->SetupAttachment(SwordMesh);		// 콜리전을 검 메쉬에 붙임

	// 콜리전 위치 및 방향, 크기 설정
	SwordCollision->SetRelativeLocation(FVector(0.0f, 450.0f, 0.0f));
	SwordCollision->SetRelativeRotation(FRotator(0.0f,0.0f,90.0f));
	SwordCollision->SetRelativeScale3D(FVector(1.0f, 1.0f, 8.0f));

	// 검 메쉬의 크기 설정
	SwordMesh->SetRelativeScale3D(FVector(0.25f, 0.25f, 0.25f));


	Timer = 0.0f;		// 타이머 초기화

	Tags.Add(FName(TEXT("PlayerSword")));		// 생성한 방패를 'PlayerSword'란 이름으로 태그를 줌
}

// Called when the game starts or when spawned
void APlayerSword::BeginPlay()
{
	Super::BeginPlay();
	
	static UMaterialParameterCollection* Collection = Cast<UMaterialParameterCollection>(StaticLoadObject(UMaterialParameterCollection::StaticClass(), NULL,		// 머테리얼 콜렉션 찾기
		TEXT("MaterialParameterCollection'/Game/Assets/Equipment/Texture/EquipmentMaterialCollection.EquipmentMaterialCollection'"), NULL, LOAD_None, NULL));

	if (Collection)
	{
		//UE_LOG(LogTemp, Warning, TEXT("dd %s"), *Collection->GetName());
		CollectionInstance = GetWorld()->GetParameterCollectionInstance(Collection);		// 찾은 콜렉션을 콜렉션인스턴스에 저장
		CollectionInstance->SetScalarParameterValue(FName("Opacity_Sword"), 0.75);		// 'Opacity_Sword'값을 가진 파라미터 값을 세팅
	}

	if (SwordMesh)	
	{
		SwordMesh->OnComponentBeginOverlap.AddDynamic(this, &APlayerSword::OnSwordOverlap);		// 오버랩 이벤트를 발생시킬 수 있도록 설정
	}
}

// Called every frame
void APlayerSword::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	Timer += DeltaTime;		// 타이머
}

void APlayerSword::OnSwordOverlap(UPrimitiveComponent * OverlappedComp, AActor * OtherActor, UPrimitiveComponent * OtherComp,
	int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult)		// 오버랩이벤트시 실행할 것들
{
	if (OtherActor->ActorHasTag("Monster"))		// 오버랩된 액터가 'Monster'라는 태그를 가지고 있으면 실행
	{
		UE_LOG(LogClass, Warning, TEXT("22222222222222222"));
		if (Timer >= 0.5f)			// 타이머가 0.5 이상의 수를 가지고 있을 때 실행 (조건1)
		{
			if (SwordMesh->GetPhysicsLinearVelocity().Size() >= 200.0f)		// 선속도의 크기가 200 이상일 때만 공격 판정이 일어남 (조건2)
			{
				Timer = 0.0f;		// 공격 판정이 일어났을 때 타이머 0으로

				if (SwordMesh->GetPhysicsLinearVelocity().Size() <= 500)		// 선속도의 크기가 500이하일 때 데미지 10 (조건4)
					Damage = 10.0f;
				else			// 선속도의 크기가 500초과일 때 데미지 15 (조건4)
					Damage = 15.0f;

				//AMotionControllerCharacter* MyCharacter = Cast<AMotionControllerCharacter>(GetOwner());
				//ULeftHandWidget* MyStateUI = Cast<ULeftHandWidget>(MyCharacter->LeftHand->Shield->CharacterStateWidget);
				// 이하 캐릭터 스테미너 감소
				// 이하 UI스테미너 감소
 				 
				UGameplayStatics::ApplyDamage(OtherActor, Damage, UGameplayStatics::GetPlayerController(GetWorld(), 0), this, nullptr);		// 오버랩된 액터에 데미지 전달
				// (오버랩발생된 액터, 데미지, 데미지를가한 주체, 실제 데미지를 가한주체, 데미지종류클래스)
			}
		}
	}	
}

void APlayerSword::ConvertOfOpacity(float opacity)		// Opacity값 세팅(캐릭터에서 호출)
{
	static UMaterialParameterCollection* Collection = Cast<UMaterialParameterCollection>(StaticLoadObject(UMaterialParameterCollection::StaticClass(), NULL,		// 머테리얼 콜렉션 찾기
		TEXT("MaterialParameterCollection'/Game/Assets/Equipment/Texture/EquipmentMaterialCollection.EquipmentMaterialCollection'"), NULL, LOAD_None, NULL));

	if (Collection)
	{
		//UE_LOG(LogTemp, Warning, TEXT("dd %s"), *Collection->GetName());
		CollectionInstance = GetWorld()->GetParameterCollectionInstance(Collection);		// 찾은 콜렉션을 콜렉션인스턴스에 저장
		CollectionInstance->SetScalarParameterValue(FName("Opacity_Sword"), opacity);		// 'Opacity_Sword'값을 가진 파라미터 값을 세팅
	}
}
