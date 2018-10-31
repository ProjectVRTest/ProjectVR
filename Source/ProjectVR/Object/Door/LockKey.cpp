// Fill out your copyright notice in the Description page of Project Settings.

#include "LockKey.h"
#include "Components/StaticMeshComponent.h"
#include "UObject/ConstructorHelpers.h"
#include "Engine/StaticMesh.h"
#include "Kismet/GameplayStatics.h"							// 캐릭터 찾기
#include "MyCharacter/MotionControllerCharacter.h"
// Sets default values
ALockKey::ALockKey()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	
	Key = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Key"));
	Key->SetupAttachment(RootComponent);

	static ConstructorHelpers::FObjectFinder<UStaticMesh>SM_Key(TEXT("StaticMesh'/Game/Assets/CharacterEquipment/Item/LockAndKey/Key/Mesh/SM_Key.SM_Key'"));		// 레퍼런스 경로로 방패 매쉬를 찾음
	if (SM_Key.Succeeded())		// 검 메쉬를 찾았을 경우 실행
	{
		Key->SetStaticMesh(SM_Key.Object);			// 스태틱 메쉬에 검 모양 설정
	}

	Key->SetEnableGravity(false);
	Key->bGenerateOverlapEvents = true;
	Key->SetCollisionProfileName("OverlapAll");
	Tags.Add("Key");
}

// Called when the game starts or when spawned
void ALockKey::BeginPlay()
{
	Super::BeginPlay();

	Key->OnComponentBeginOverlap.AddDynamic(this, &ALockKey::OnOverlap);		// 오버랩 이벤트를 발생시킬 수 있도록 설정
}

// Called every frame
void ALockKey::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ALockKey::OnOverlap(UPrimitiveComponent * OverlappedComp, AActor * OtherActor, UPrimitiveComponent * OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult)
{
	if (OtherActor->ActorHasTag("RightHand") || OtherActor->ActorHasTag("LeftHand"))
	{
		AMotionControllerCharacter* Character = Cast<AMotionControllerCharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));
		
		if (!Character->bHasKey)
		{
			Character->bHasKey = true;
			Destroy(this);
		}
	}
}

