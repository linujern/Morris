// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "DOGStatUpgradePickups.h"
#include "DOGWeaponPickup.h"
#include "DOGWorldInteractable.h"
#include "GameFramework/Actor.h"
#include "Morris/Data/DOGItemData.h"
#include "DOGChest.generated.h"

UCLASS()
class MORRIS_API ADOGChest : public ADOGWorldInteractable
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	ADOGChest();

	virtual void Interact(AActor* Caller) override;

	UFUNCTION()
	void LoadedAsset(FPrimaryAssetId LoadedId, FVector SpawnLocation);
	
	UFUNCTION(BlueprintCallable)
	void EnableChest();
	UFUNCTION(BlueprintCallable)
	void DisableChest();
	
	UFUNCTION(BlueprintImplementableEvent)
	void OnTriggerChest(UDOGItemData* ItemData);

	UFUNCTION(BlueprintCallable)
	AActor* SpawnItemFromData(UDOGItemData* ItemData, FTransform Transform);

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	TSubclassOf<ADOGStatUpgradePickups> StatClass = nullptr;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	TSubclassOf<ADOGWeaponPickup> WeaponClass = nullptr;

	// True for weapon chest false for stat chest
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	bool bWeaponChest = true;
	
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;
};
