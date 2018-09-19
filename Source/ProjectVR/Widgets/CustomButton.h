// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/Button.h"
#include "CustomButton.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTVR_API UCustomButton : public UButton
{
	GENERATED_BODY()

	UCustomButton();

#if WITH_EDITOR
	virtual const FText GetPaletteCategory() override;
#endif
	
	
};
