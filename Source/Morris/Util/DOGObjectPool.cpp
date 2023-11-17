// Fill out your copyright notice in the Description page of Project Settings.


#include "DOGObjectPool.h"

AActor* UDOGObjectPool::TryGetFromPool()
{
	//RETURNS NULLPTR IF POOL IS EMPTY
	if(ActorsInPool.Num() > 0)
	{
		AActor* OutActor = ActorsInPool[0];
		ActorsInPool.RemoveAt(0);
		OutActor->SetActorTickEnabled(true);
		OutActor->SetActorEnableCollision(true);
		OutActor->SetActorHiddenInGame(false);
		return OutActor;
	}
	return nullptr;
}

void UDOGObjectPool::AddToPool(AActor* Actor)
{
	Actor->SetActorHiddenInGame(true);
	Actor->SetActorEnableCollision(false);
	Actor->SetActorLocation(FVector(0,0,100000));
	Actor->SetActorTickEnabled(false);
	
	ActorsInPool.Add(Actor);
}
