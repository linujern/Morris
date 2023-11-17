// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "DOGWorldInteractable.h"
#include "Morris/Data/DOGPowerupData.h"
#include "DOGPowerPickup.generated.h"

/**
 * 
 */
UCLASS()
class MORRIS_API ADOGPowerPickup : public ADOGWorldInteractable
{
	GENERATED_BODY()
public:
	ADOGPowerPickup();

	virtual void BeginPlay() override;
	virtual void Interact(AActor* Caller) override;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Power Pickup")
	TSoftObjectPtr<UDOGPowerupData> PowerPickupData = nullptr;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Power Pickup")
	UDOGPowerupData* LoadedPowerPickupData = nullptr;

};
