// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/Image.h"
#include "Components/ProgressBar.h"
#include "Components/TextBlock.h"
#include "DOGHud.generated.h"

class ADOGPlayer;
/**
 * 
 */
UCLASS(Abstract)
class MORRIS_API UDOGHud : public UUserWidget
{
	GENERATED_BODY()

public:
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(BindWidget))
	UTextBlock* AmmoCountPrimary;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(BindWidget))
	UTextBlock* AmmoCountSecondary;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(BindWidget))
	UImage* PrimaryItemIcon;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(BindWidget))
	UImage* SecondaryItemIcon;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(BindWidget))
	UProgressBar* HealthProgressBar;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(BindWidget))
	UTextBlock* ScrapMetalAmount;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(BindWidget))
	UTextBlock* DashAmount;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta=(ExposeOnSpawn))
	ADOGPlayer* PlayerReference;

	UFUNCTION(BlueprintImplementableEvent)
	void OnInteractLocal(AActor* Item);

	UFUNCTION(BlueprintImplementableEvent)
	void OnSwapEquipment(AActor* NewPrimary, AActor* NewSecondary);

	UFUNCTION(BlueprintImplementableEvent)
	void OnDropHeldEquipment();

	UFUNCTION(BlueprintImplementableEvent)
	void OnUpdatedDashes(float CurrentDashes);
	
protected:
	
	virtual void NativeConstruct() override;
	
};
