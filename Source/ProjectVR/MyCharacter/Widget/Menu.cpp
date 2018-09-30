// Fill out your copyright notice in the Description page of Project Settings.

#include "Menu.h"

#include "UObject/ConstructorHelpers.h"					// 레퍼런스 경로
#include "Components/WidgetComponent.h"
#include "Blueprint/UserWidget.h"
#include "Components/BoxComponent.h"

// 위젯을 이용해서 하나의 오브젝트를 만듦

// Sets default values
AMenu::AMenu()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Scene = CreateDefaultSubobject<USceneComponent>(TEXT("Scene"));
	Scene->SetupAttachment(RootComponent);

	Menu = CreateDefaultSubobject<UWidgetComponent>(TEXT("Menu"));
	Menu->SetupAttachment(Scene);

	RangeBox = CreateDefaultSubobject<UBoxComponent>(TEXT("RangeBox"));
	RangeBox->SetupAttachment(Menu);


	static ConstructorHelpers::FClassFinder<UUserWidget> BP_Widget(TEXT("WidgetBlueprint'/Game/Blueprints/UI/Menu/MenuTest.MenuTest_C'"));
	if (BP_Widget.Succeeded())
	{
		Menu->SetWidgetClass(BP_Widget.Class);
	}

	Menu->SetRelativeScale3D(FVector(0.3f, 0.3f, 0.3f));
	Menu->SetDrawSize(FVector2D(630.0f, 210.0f));
	Menu->SetBlendMode(EWidgetBlendMode::Transparent);
	// 타겟을 잡으면서 손과 오버랩되는 조건
	RangeBox->SetCollisionProfileName("Custom...");
	RangeBox->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	RangeBox->SetCollisionObjectType(ECollisionChannel::ECC_WorldDynamic);
	RangeBox->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECollisionResponse::ECR_Overlap);
	RangeBox->SetCollisionResponseToChannel(ECollisionChannel::ECC_Camera, ECollisionResponse::ECR_Overlap);
	RangeBox->SetCollisionResponseToChannel(ECollisionChannel::ECC_Destructible, ECollisionResponse::ECR_Overlap);
	RangeBox->SetCollisionResponseToChannel(ECollisionChannel::ECC_WorldStatic, ECollisionResponse::ECR_Overlap);
	RangeBox->SetCollisionResponseToChannel(ECollisionChannel::ECC_WorldDynamic, ECollisionResponse::ECR_Overlap);
	RangeBox->SetCollisionResponseToChannel(ECollisionChannel::ECC_PhysicsBody, ECollisionResponse::ECR_Overlap);
	RangeBox->SetCollisionResponseToChannel(ECollisionChannel::ECC_Vehicle, ECollisionResponse::ECR_Overlap);
	RangeBox->SetCollisionResponseToChannel(ECollisionChannel::ECC_Visibility, ECollisionResponse::ECR_Ignore);

	RangeBox->SetRelativeScale3D(FVector(2.0f, 5.7f, 2.7f));
	RangeBox->SetRelativeLocation(FVector(50.0f, 0.0f, 0.0f));
	RangeBox->bHiddenInGame = false;

	RangeBox->ComponentTags.Add(FName(TEXT("GrabRange")));
	Tags.Add(FName(TEXT("DisregardForLeftHand")));
	Tags.Add(FName(TEXT("DisregardForRightHand")));
}

// Called when the game starts or when spawned
void AMenu::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AMenu::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

