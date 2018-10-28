// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "CustomMainWidget.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTVR_API UCustomMainWidget : public UUserWidget
{
	GENERATED_BODY()
	
	virtual void NativeConstruct() override; //»ý¼ºÀÚ

	UPROPERTY()
		class UCustomButton* CB_1;
	UPROPERTY()
		class UCustomButton* CB_2;
	UPROPERTY()
		class UCustomButton* CB_3;

	UFUNCTION()
		void OnClickedCB_1();
	UFUNCTION()
		void OnClickedCB_2();
	UFUNCTION()
		void OnClickedCB_3();
	
};
