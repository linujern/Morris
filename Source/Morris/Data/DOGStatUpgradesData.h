// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "DOGItemData.h"
#include "UObject/Object.h"
#include "DOGStatUpgradesData.generated.h"

/**
 * 
 */
UCLASS()
class MORRIS_API UDOGStatUpgradesData : public UDOGItemData
{
	GENERATED_BODY()

public:
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Stats Upgrades",
	meta=(ToolTip="Stat type, VERY IMPORTANT!"))
	TEnumAsByte<EStatUpgrades> StatType = HEALTHUPGRADE;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Stats Upgrades",
	meta=(ToolTip="Stat Increase"))
	float StatIncrease = 10.f;
	
	
};
