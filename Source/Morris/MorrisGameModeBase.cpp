// Copyright Epic Games, Inc. All Rights Reserved.


#include "MorrisGameModeBase.h"

#include "Kismet/GameplayStatics.h"

AMorrisGameModeBase::AMorrisGameModeBase()
{
}

void AMorrisGameModeBase::BeginPlay()
{
	Super::BeginPlay();
	//TODO: Create pool
	PoolManager = NewObject<UDOGObjectPoolManager>();
	PoolManager->CreatePool("BulletPool");
}
