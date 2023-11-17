// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "DOGInputIconData.generated.h"

/**
 * 
 */
UCLASS()
class MORRIS_API UDOGInputIconData : public UPrimaryDataAsset
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Input|Playstation")
	UTexture2D* PlaystationIcon;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Input|Xbox")
	UTexture2D* XboxIcon;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Input|PC")
	UTexture2D* PCIcon;
};
