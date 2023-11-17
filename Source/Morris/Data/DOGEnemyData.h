// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "DOGData.h"
#include "UObject/Object.h"
#include "DOGEnemyData.generated.h"

/**
 * 
 */



UCLASS()
class MORRIS_API UDOGEnemyData : public UPrimaryDataAsset
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Enemy|BoomBoi")
	float Health;

	
};
