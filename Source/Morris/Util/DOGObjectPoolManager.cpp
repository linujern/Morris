// Fill out your copyright notice in the Description page of Project Settings.


#include "DOGObjectPoolManager.h"

UDOGObjectPoolManager::UDOGObjectPoolManager()
{
	Pools.Reserve(20);
}

TWeakObjectPtr<UDOGObjectPool> UDOGObjectPoolManager::CreatePool(FString Name)
{
	TWeakObjectPtr<UDOGObjectPool> Pool = TWeakObjectPtr<UDOGObjectPool>(NewObject<UDOGObjectPool>());
	Pools.Add(Name, Pool.Get());
	return Pool;
}

UDOGObjectPool* UDOGObjectPoolManager::GetPool(FString Name)
{
	UDOGObjectPool* ReturnValue = Pools.FindRef(Name);
	return ReturnValue;
}
