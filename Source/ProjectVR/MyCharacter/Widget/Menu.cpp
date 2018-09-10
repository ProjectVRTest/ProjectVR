// Fill out your copyright notice in the Description page of Project Settings.

#include "Menu.h"

#include "UObject/ConstructorHelpers.h"					// 레퍼런스 경로
#include "Components/WidgetComponent.h"
#include "Blueprint/UserWidget.h"

// Sets default values
AMenu::AMenu()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	//Scene = CreateDefaultSubobject<USceneComponent>(TEXT("Scene"));
	//Scene->SetupAttachment(RootComponent);

	Menu = CreateDefaultSubobject<UWidgetComponent>(TEXT("Menu"));
	Menu->SetupAttachment(RootComponent);

	static ConstructorHelpers::FObjectFinder<UUserWidget> BP_Widget(TEXT("WidgetBlueprint'/Game/Blueprints/UI/Menu/MenuTest.MenuTest_C'"));
	if (BP_Widget.Succeeded())
	{
		Menu->SetWidget(BP_Widget.Object);
	}

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

