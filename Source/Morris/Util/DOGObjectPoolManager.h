// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "DOGObjectPool.h"
#include "UObject/Object.h"
#include "DOGObjectPoolManager.generated.h"

/**
 * 
 */
UCLASS()
class MORRIS_API UDOGObjectPoolManager : public UObject
{
	GENERATED_BODY()

public:
	UPROPERTY()
	TMap<FString, UDOGObjectPool*> Pools;

	UDOGObjectPoolManager();

	UFUNCTION()
	TWeakObjectPtr<UDOGObjectPool> CreatePool(FString Name);

	UFUNCTION()
	UDOGObjectPool* GetPool(FString Name);
	
};
