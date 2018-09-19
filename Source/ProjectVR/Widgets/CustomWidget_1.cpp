// Fill out your copyright notice in the Description page of Project Settings.

#include "CustomWidget_1.h"
#include "Widgets/CustomButton.h"

// À§Á¬ ÀÚÃ¼

void UCustomWidget_1::NativeConstruct()
{
	Super::NativeConstruct();

	CB_1 = Cast<UCustomButton>(GetWidgetFromName(TEXT("ExitButton")));

	CB_1->OnClicked.AddDynamic(this, &UCustomWidget_1::OnClickedCheck);
}

void UCustomWidget_1::OnClickedCheck()
{
	UE_LOG(LogTemp, Log, TEXT("Press CustomButton!!!"));
}
