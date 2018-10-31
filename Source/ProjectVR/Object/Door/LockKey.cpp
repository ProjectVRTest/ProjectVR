// Fill out your copyright notice in the Description page of Project Settings.

#include "LockKey.h"
#include "Components/StaticMeshComponent.h"
#include "UObject/ConstructorHelpers.h"
#include "Engine/StaticMesh.h"
#include "Kismet/GameplayStatics.h"							// ĳ���� ã��
#include "MyCharacter/MotionControllerCharacter.h"
// Sets default values
ALockKey::ALockKey()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	
	Key = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Key"));
	Key->SetupAttachment(RootComponent);

	static ConstructorHelpers::FObjectFinder<UStaticMesh>SM_Key(TEXT("StaticMesh'/Game/Assets/CharacterEquipment/Item/LockAndKey/Key/Mesh/SM_Key.SM_Key'"));		// ���۷��� ��η� ���� �Ž��� ã��
	if (SM_Key.Succeeded())		// �� �޽��� ã���� ��� ����
	{
		Key->SetStaticMesh(SM_Key.Object);			// ����ƽ �޽��� �� ��� ����
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

	Key->OnComponentBeginOverlap.AddDynamic(this, &ALockKey::OnOverlap);		// ������ �̺�Ʈ�� �߻���ų �� �ֵ��� ����
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

