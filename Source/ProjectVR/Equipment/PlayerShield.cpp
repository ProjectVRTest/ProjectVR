﻿// Fill out your copyright notice in the Description page of Project Settings.

#include "PlayerShield.h"
#include "Components/WidgetComponent.h"			// 위젯 컴포넌트
#include "Components/StaticMeshComponent.h"		// 스태틱 메쉬 컴포넌트
#include "UObject/ConstructorHelpers.h"				// ConstructorHelpers 사용

#include "Runtime/Engine/Classes/Materials/MaterialParameterCollectionInstance.h"
#include "Runtime/Engine/Classes/Materials/MaterialParameterCollection.h"
#include "Engine/StaticMesh.h"

// Sets default values
APlayerShield::APlayerShield()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	
	/* 스태틱 매쉬 컴포넌트 생성 */
	ShieldMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ShieldMesh"));
	ShieldMesh->SetCollisionProfileName(TEXT("OverlapOnlyPawn"));			// 캐릭터와의 충돌을 피하기위해서 Pawn(캐릭터)만 Overlap되도록 설정
	SetRootComponent(ShieldMesh);

	ShieldMesh->SetRelativeScale3D(FVector(-1.0f, 1.0f, 1.0f));

	static ConstructorHelpers::FObjectFinder<UStaticMesh>SM_Shield(TEXT("StaticMesh'/Game/Assets/Equipment/Shield/Mesh/SM_Shield.SM_Shield'"));		// 레퍼런스 경로로 방패 매쉬를 찾음
	if (SM_Shield.Succeeded())		// 방패 메쉬를 찾았을 경우 실행
	{
		ShieldMesh->SetStaticMesh(SM_Shield.Object);		// 스태틱 메쉬에 방패 모양 설정
	}

	Tags.Add(FName(TEXT("PlayerShield")));		// 생성한 방패를 'PlayerShield'란 이름으로 태그를 줌
	Tags.Add(FName(TEXT("DisregardForLeftHand")));
	Tags.Add(FName(TEXT("DisregardForRightHand")));
}

// Called when the game starts or when spawned
void APlayerShield::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void APlayerShield::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	//UE_LOG(LogTemp, Log, TEXT(" ** %f"), ShieldMesh->GetPhysicsLinearVelocity().Size());
}

void APlayerShield::ConvertOfOpacity(float opacity)		// Opacity값 세팅(캐릭터에서 호출)
{
	if (ShieldMesh)
	{
		ShieldMesh->SetScalarParameterValueOnMaterials(FName(TEXT("Min")), opacity);
		ShieldMesh->SetScalarParameterValueOnMaterials(FName(TEXT("Max")), opacity);
	}
	//static UMaterialParameterCollection* Collection = Cast<UMaterialParameterCollection>(StaticLoadObject(UMaterialParameterCollection::StaticClass(), NULL,		// 머테리얼 콜렉션 찾기
	//	TEXT("Material'/Game/Assets/Equipment/Shield/Materials/ShieldMaterial.ShieldMaterial'"), NULL, LOAD_None, NULL));	

	//if (Collection)
	//{
	//	//UE_LOG(LogTemp, Warning, TEXT("dd %s"), *Collection->GetName());
	//	CollectionInstance = GetWorld()->GetParameterCollectionInstance(Collection);		// 찾은 콜렉션을 콜렉션인스턴스에 저장
	//	CollectionInstance->SetScalarParameterValue(FName("Opacity_Shield"), opacity);		// 'Opacity_Sword'값을 가진 파라미터 값을 세팅
	//}
}

