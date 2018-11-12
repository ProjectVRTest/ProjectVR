// Fill out your copyright notice in the Description page of Project Settings.

#include "CustomWidget_1.h"
#include "Widgets/CustomButton.h"
#include "Kismet/GameplayStatics.h"
#include "MyCharacter/MotionControllerCharacter.h"
// À§Á¬ ÀÚÃ¼

void UCustomWidget_1::NativeConstruct()
{
	Super::NativeConstruct();

	CB_1 = Cast<UCustomButton>(GetWidgetFromName(TEXT("ExitButton")));
	CB_2 = Cast<UCustomButton>(GetWidgetFromName(TEXT("CancelButton")));

	CB_1->OnClicked.AddDynamic(this, &UCustomWidget_1::OnClickedCB_1);
	CB_2->OnClicked.AddDynamic(this, &UCustomWidget_1::OnClickedCB_2);
}

void UCustomWidget_1::OnClickedCB_1()
{
	Cast<AMotionControllerCharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0))->MoveMainScene();
}

void UCustomWidget_1::OnClickedCB_2()
{
	Cast<AMotionControllerCharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0))->GameMenu();
}