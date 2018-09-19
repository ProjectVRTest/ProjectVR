// Fill out your copyright notice in the Description page of Project Settings.

#include "CustomButton.h"
#include "UObject/ConstructorHelpers.h"
#include "UMG.h"

#define LOCTEXT_NAMESPACE "UMG"

UCustomButton::UCustomButton()
{
	SButton::FArguments ButtonDefaults;

	static ConstructorHelpers::FObjectFinder<USlateWidgetStyleAsset>CustomStyle(TEXT("SlateWidgetStyleAsset'/Game/Blueprints/UI/Button/CustomWidgetStyleAsset.CustomWidgetStyleAsset'"));
	if (CustomStyle.Succeeded())
	{
		ButtonDefaults.ButtonStyle(CustomStyle.Object);
	}
	WidgetStyle = *ButtonDefaults._ButtonStyle;
}

#if WITH_EDITOR
const FText UCustomButton::GetPaletteCategory()
{
	return LOCTEXT("", "My Custom Cuff");
}
#endif

#undef LOCTEXT_NAMESPACE