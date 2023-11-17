// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "DOGData.h"
#include "Components/Image.h"
#include "UObject/Object.h"
#include "DOGItemData.generated.h"

/**
 * 
 */
UCLASS()
class MORRIS_API UDOGItemData : public UPrimaryDataAsset
{
	GENERATED_BODY()

public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Item",
		meta=(ToolTip="Item type, VERY IMPORTANT!"))
	TEnumAsByte<EShopItemType> ShopItemType = WEAPON;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Item",
		meta=(ToolTip="Item name to be displayed in Shop."))
	FName ItemName = "Big Chungus";

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Item",
		meta=(ToolTip="Item description to be displayed in Shop."))
	FString ItemDescription = "MISSINGNO";
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Item",
		meta=(ToolTip="Rairty color to be displayed in Shop."))
	FLinearColor ItemColor = FLinearColor::White;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Item", DisplayName = "Shop Price Range",
				meta=(ToolTip="Shop price will be generated from a random number in this range. Numbers are inclusive."))
	FFloatInterval ShopPrice;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Item", DisplayName = "Shop Mesh",
				meta=(ToolTip="Shop mesh to display in shop.\n\nNote: If weapon, leave blank."))
	UStaticMesh* ShopMesh = nullptr;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Item", DisplayName = "Background Image",
				meta=(ToolTip="Image used in the background of this items display UI"))
	UTexture2D* ItemBackgroundImage = nullptr;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Item", DisplayName = "Background Image",
				meta=(ToolTip="Image used as a icon in this items display UI, to show what item this is"))
	UTexture2D* ItemIconImage = nullptr;

	virtual void PostInitProperties() override;
};
