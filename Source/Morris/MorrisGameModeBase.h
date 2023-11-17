// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "Util/DOGObjectPoolManager.h"
#include "MorrisGameModeBase.generated.h"

/**
 * 
 */
UCLASS()
class MORRIS_API AMorrisGameModeBase : public AGameModeBase
{
	GENERATED_BODY()

public:
	AMorrisGameModeBase();

	UPROPERTY()
	UDOGObjectPoolManager* PoolManager = nullptr;

	virtual void BeginPlay() override;
	
};
