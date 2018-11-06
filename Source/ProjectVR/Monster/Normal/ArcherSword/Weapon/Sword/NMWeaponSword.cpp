// Fill out your copyright notice in the Description page of Project Settings.

#include "NMWeaponSword.h"
#include "Components/StaticMeshComponent.h" //스테틱 메쉬 컴포넌트 헤더
#include "UObject/ConstructorHelpers.h" //생성자에서 오브젝트를 에디터상에서 찾을 때 필요한 헤더 
#include "Engine/StaticMesh.h" //스테틱 메쉬 헤더
#include "Components/CapsuleComponent.h" //캡슐 콜리전 헤더
#include "MyCharacter/MotionControllerCharacter.h" //플레이어 헤더
#include "Monster/Normal/ArcherSword/NormalMonster.h" //일반몬스터 헤더
#include "Kismet/GameplayStatics.h"
#include "MyCharacter/CameraLocation.h"

// Sets default values
ANMWeaponSword::ANMWeaponSword()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true; 

	SwordMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("SwordMesh")); //스테틱 메쉬 컴포넌트를 생성해주고
	SetRootComponent(SwordMesh); //루트 컴포넌트로 정한다.
	SwordMesh->SetCollisionProfileName("NoCollision"); //콜리전반응을 제거한다.

	//에디터로부터 일반몬스터 검 메쉬를 찾고
	static ConstructorHelpers::FObjectFinder<UStaticMesh>SM_Sword(TEXT("StaticMesh'/Game/Assets/CharacterEquipment/Monster/NormalMonster/Mesh/Weapon/Sword/Mesh/SM_NormalMonsterSword.SM_NormalMonsterSword'"));

	//찾는것에 성공하면
	if (SM_Sword.Succeeded())
	{
		//검 스테틱메쉬 컴포넌트에 넣는다.
		SwordMesh->SetStaticMesh(SM_Sword.Object);
	}

	//공격이 가능한 상태인지 알려주는 변수로 처음에는 꺼둔다.
	IsWeaponAttack = false;
	IsPlayerEquipment = false; 

	//오버랩반응을 구현하기 위해서 캡슐콜리전컴포넌트를 생성해주고
	SwordCollision = CreateDefaultSubobject<UCapsuleComponent>(TEXT("SwordCollision"));
	SwordCollision->SetupAttachment(SwordMesh); //스테틱메쉬 아래에 붙인다.
	SwordCollision->SetCollisionProfileName(TEXT("OverlapAll")); //콜리전반응을 OverlapAll로 정해준다.
	SwordCollision->SetRelativeScale3D(FVector(0.6f, 0.6f, 1.2f)); //크기를 정한다.
	
	SwordCollision->bHiddenInGame = false; //게임상에서 보이게 해준다.

	//일반몬스터검을 의미하는 태그를 달아준다.
	Tags.Add(FName(TEXT("NormalMonsterSword")));
	//왼손과 오른손에 반응이 없게 태그를 달아준다.
	Tags.Add(FName(TEXT("DisregardForRightHand")));
	Tags.Add(FName(TEXT("DisregardForLeftHand")));	
}

// Called when the game starts or when spawned
void ANMWeaponSword::BeginPlay()
{
	Super::BeginPlay();
	
	//검 콜리전이 있으면
	if (SwordCollision)
	{
		//검콜리전에 SwordBeginOverlap함수를 바인딩해준다.
		SwordCollision->OnComponentBeginOverlap.AddDynamic(this, &ANMWeaponSword::SwordBeginOverlap);
	}
}

// Called every frame
void ANMWeaponSword::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	ANormalMonster* NormalMonster = Cast<ANormalMonster>(GetAttachParentActor());

	if (NormalMonster)
	{
		if (NormalMonster->CurrentState == ENormalMonsterState::Dead)
		{
			SwordMesh->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
			SwordMesh->SetSimulatePhysics(true);
			DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);
			InitialLifeSpan = 4.0f;
		}
	}
}

void ANMWeaponSword::SwordBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult)
{
	if (IsWeaponAttack) //공격이 가능한 상태이고
	{
		if (OtherComp->ComponentHasTag(TEXT("CameraLocation")))//부딪힌 상대방의 태그가 Character이면
		{
			ACameraLocation* CameraLocation = Cast<ACameraLocation>(OtherComp->GetOwner());

			if (CameraLocation)
			{
				UGameplayStatics::ApplyDamage(OtherComp->GetOwner()->GetAttachParentActor(), Damage, nullptr, this, nullptr);
			}
		}
	}
}