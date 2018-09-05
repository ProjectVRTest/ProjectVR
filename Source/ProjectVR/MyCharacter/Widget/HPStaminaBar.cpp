// Fill out your copyright notice in the Description page of Project Settings.

#include "HPStaminaBar.h"
#include "Components/BoxComponent.h"

// Sets default values
AHPStaminaBar::AHPStaminaBar()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// HP
	HPScene = CreateDefaultSubobject<USceneComponent>(TEXT("HPScene"));
	HPScene->SetupAttachment(RootComponent);
	// HP
	HPBar = CreateDefaultSubobject<UBoxComponent>(TEXT("HPBar"));
	HPBar->SetupAttachment(HPScene);

	// Stamina
	SteminaScene = CreateDefaultSubobject<USceneComponent>(TEXT("SteminaScene"));
	SteminaScene->SetupAttachment(RootComponent);
	// Stamina
	StaminaBar = CreateDefaultSubobject<UBoxComponent>(TEXT("SteminaBar"));
	StaminaBar->SetupAttachment(SteminaScene);

	// 체력 바 및 스테미너 바 텍스처 예정

	// 바 사이즈 설정
	HPBarXSize = 0.8f;
	HPBarZSize = 0.04f;
	SteminaBarXSize = 0.8f;
	SteminaBarZSize = 0.04f;

	// 사이즈 및 위치 지정
	HPScene->SetRelativeLocation(FVector(-(HPBarXSize / 2.0f)*100.0f, 0.0f, 0.0f));
	HPScene->SetRelativeScale3D(FVector(1.0f, 1.0f, 1.0f));
	SteminaScene->SetRelativeLocation(FVector(-(HPBarXSize / 2.0f)*100.0f, 0.0f, -(HPBarXSize * 200)));
	SteminaScene->SetRelativeScale3D(FVector(1.0f, 1.0f, 1.0f));

	HPBar->SetRelativeScale3D(FVector(0.8f, 0.007f, HPBarZSize));
	HPBar->SetRelativeLocation(FVector((HPBarXSize / 2.0f)*100.0f, 0.0f, 0.0f));
	StaminaBar->SetRelativeScale3D(FVector(0.8f, 0.007f, SteminaBarZSize));
	StaminaBar->SetRelativeLocation(FVector((HPBarXSize / 2.0f)*100.0f, 0.0f, 0.0f));
}

// Called when the game starts or when spawned
void AHPStaminaBar::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AHPStaminaBar::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

