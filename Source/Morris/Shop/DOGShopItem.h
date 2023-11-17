// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Morris/Data/DOGData.h"
#include "Morris/Interactables/DOGWorldInteractable.h"
#include "Morris/UI/DOGItemUI.h"
#include "DOGShopItem.generated.h"

UCLASS()
class MORRIS_API ADOGShopItem : public ADOGWorldInteractable
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	ADOGShopItem();

	//TODO: FIX lmao
	/** Static mesh */
	//UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	//UStaticMeshComponent* StaticMeshComponent;

	//TODO: UI

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(ToolTip="What type of items are randomly generated in this Shop."))
	TEnumAsByte<EShopType> ShopType;

	UPROPERTY()
	TEnumAsByte<EShopItemType> ShopItemType;
	
	//UPROPERTY(EditAnywhere, BlueprintReadWrite)
	//TSubclassOf<UDOGItemUI> ShopHudWidgetClass = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UDOGItemData* ItemData;

	//UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	//UDOGItemUI* HudWidget = nullptr;
	
	UPROPERTY()
	float ItemPrice = 0.f;

	UPROPERTY()
	FName ItemName;

	UPROPERTY()
	FLinearColor ItemColor;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	USoundBase* PurchaseSound = nullptr;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	USoundBase* PurchaseVoicelines = nullptr;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	USoundBase* PurchaseNoMoneySound = nullptr;
	
	UFUNCTION(BlueprintCallable)
	void GetItemData(UDOGItemData* Data);
	
	virtual void Interact(AActor* Caller) override;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;
};
