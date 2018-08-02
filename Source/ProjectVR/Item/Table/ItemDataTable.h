// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "ItemDataTable.generated.h"

UENUM(BlueprintType)
enum class EItemType : uint8
{
	UnKnown = 0 UMETA(DisplayName = "UnKnown"),
	Consumable = 1 UMETA(DisplayName = "Consumable"),
	Equip = 2 UMETA(DisplayName = "Equip")
};

UENUM(BlueprintType)
enum class EEquipSlot : uint8
{
	UnKnown = 0 UMETA(DisplayName = "UnKnown"),
	WeaponLeft = 1 UMETA(DisplayName = "WeaponLeft"),
	WeaponRight = 2 UMETA(DisplayName = "WeaponRight")
};

USTRUCT(BlueprintType)
struct PROJECTVR_API FItemDataTable : public FTableRowBase
{
	GENERATED_USTRUCT_BODY()
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data")
		int ItemIndex;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data")
		FString ItemName;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data")
		EItemType ItemType;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data")
		EEquipSlot EquipSlot;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data")
		int ItemCurrentAmount;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data")
		int ItemMaxAmount;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data")
		int ItemUsePoint;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data")
		TAssetPtr<class UStaticMesh> ItemMesh;
	

};
