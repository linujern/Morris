// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "DOGObjectPool.generated.h"

/**
 * 
 */

UCLASS()
class MORRIS_API UDOGObjectPool : public UObject
{
	GENERATED_BODY()

public:
	UPROPERTY()
	TArray<AActor*> ActorsInPool;

	UFUNCTION()
	AActor* TryGetFromPool();

	UFUNCTION()
	void AddToPool(AActor* Actor);
	
};
