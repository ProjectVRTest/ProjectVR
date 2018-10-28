// Fill out your copyright notice in the Description page of Project Settings.

#include "CustomMainWidget.h"
#include "Widgets/CustomButton.h"

void UCustomMainWidget::NativeConstruct()
{
	CB_1 = Cast<UCustomButton>(GetWidgetFromName(TEXT("GameStartButton")));
	CB_2 = Cast<UCustomButton>(GetWidgetFromName(TEXT("TutorialButton")));
	CB_3 = Cast<UCustomButton>(GetWidgetFromName(TEXT("GameExitButton")));

	CB_1->OnClicked.AddDynamic(this, &UCustomMainWidget::OnClickedCB_1);
	CB_2->OnClicked.AddDynamic(this, &UCustomMainWidget::OnClickedCB_2);
	CB_3->OnClicked.AddDynamic(this, &UCustomMainWidget::OnClickedCB_3);
}

void UCustomMainWidget::OnClickedCB_1()
{
	UE_LOG(LogTemp, Log, TEXT("Press Start"));
}

void UCustomMainWidget::OnClickedCB_2()
{
	UE_LOG(LogTemp, Log, TEXT("Press Tutorial"));
}

void UCustomMainWidget::OnClickedCB_3()
{
	UE_LOG(LogTemp, Log, TEXT("Press Exit"));
}
