// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/Image.h"
#include "DOGSign.generated.h"

/**
 * 
 */
UCLASS(Abstract)
class MORRIS_API UDOGSign : public UUserWidget
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(BindWidget))
	UImage* DoorImage;
protected:
	virtual void NativeConstruct() override;

};
