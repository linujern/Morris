// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Morris/Generation/DOGRoom.h"
#include "DOGData.h"
#include "Asset/PrefabricatorAsset.h"
#include "DOGRoomData.generated.h"

/**
 * 
 */
UCLASS()
class MORRIS_API UDOGRoomData : public UPrimaryDataAsset
{
	GENERATED_BODY()

public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	bool bShouldBeGenerated = true;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	TEnumAsByte<ERoomType> RoomType;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	TSubclassOf<ADOGRoom> RoomObject = nullptr;
	
};
