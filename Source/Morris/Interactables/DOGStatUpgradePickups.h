// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "DOGWorldInteractable.h"
#include "Morris/Data/DOGStatUpgradesData.h"
#include "UObject/Object.h"
#include "DOGStatUpgradePickups.generated.h"

/**
 * 
 */
UCLASS()
class MORRIS_API ADOGStatUpgradePickups : public ADOGWorldInteractable
{
	GENERATED_BODY()
public:
	ADOGStatUpgradePickups();

	virtual void BeginPlay() override;
	virtual void Interact(AActor* Caller) override;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="StatPickup")
	TSoftObjectPtr<UDOGStatUpgradesData> StatUpgradesData = nullptr;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="StatPickup")
	UDOGStatUpgradesData* LoadedStatsData = nullptr;
};
