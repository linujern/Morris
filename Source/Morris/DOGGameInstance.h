// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Data/DOGData.h"
#include "UObject/Object.h"
#include "DOGGameInstance.generated.h"

/**
 * 
 */
UCLASS()
class MORRIS_API UDOGGameInstance : public UGameInstance
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TEnumAsByte<EInputType> InputType = PC;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	float ScrapMetal = 0.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		bool bTutorial = true;
};
