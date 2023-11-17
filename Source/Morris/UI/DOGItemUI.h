// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/Image.h"
#include "Components/Overlay.h"
#include "Components/ProgressBar.h"
#include "Components/TextBlock.h"
#include "DOGItemUI.generated.h"

class ADOGWorldInteractable;

//class ADOGShopItem;
/**
 * 
 */
UCLASS(Abstract)
class MORRIS_API UDOGItemUI : public UUserWidget
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(BindWidget))
	UImage* BackgroundImage;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(BindWidget))
	UImage* IconImage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(BindWidget))
	UTextBlock* CostText;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(BindWidget))
	UTextBlock* NameText;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(BindWidget))
	UTextBlock* TypeText;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(BindWidget))
	UTextBlock* DetailText;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(BindWidget))
	UProgressBar* DamageBar;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(BindWidget))
	UProgressBar* FireRateBar;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(BindWidget))
	UProgressBar* RangeBar;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(BindWidget))
	UProgressBar* MultiShotBar;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta=(ExposeOnSpawn))
	ADOGWorldInteractable* ItemReference;
	
protected:
	virtual void NativeConstruct() override;
};
