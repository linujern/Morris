// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/Image.h"
#include "Morris/Data/DOGInputIconData.h"
#include "Morris/Player/DOGPlayer.h"
#include "DOGBtnIIcon.generated.h"

/**
 * 
 */
UCLASS(Abstract)
class MORRIS_API UDOGBtnIIcon : public UUserWidget
{
	GENERATED_BODY()

public:
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta=(BindWidget))
	UImage* InputIcon;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(ExposeOnSpawn))
	ADOGPlayer* Player = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(ExposeOnSpawn="true"))
	UDOGInputIconData* InputData = nullptr;
	
protected:
	virtual void NativeConstruct() override;

	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;
	
};
