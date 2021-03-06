// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "CustomWidget_1.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTVR_API UCustomWidget_1 : public UUserWidget
{
	GENERATED_BODY()
	
		virtual void NativeConstruct() override; //������

	UPROPERTY()
		class UCustomButton* CB_1;
	UPROPERTY()
		class UCustomButton* CB_2;

	UFUNCTION()
		void OnClickedCB_1();
	UFUNCTION()
		void OnClickedCB_2();
	
};
