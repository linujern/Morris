// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/StreamableManager.h"
#include "GameFramework/Actor.h"
#include "Morris/Data/DOGRoomData.h"
#include "DOGGenerator.generated.h"

/** Generates a level */
UCLASS(Blueprintable, BlueprintType)
class ADOGGenerator : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	ADOGGenerator();

	/** Adds Asynchronously loaded rooms */
	UFUNCTION()
	void OnRoomLoaded(FPrimaryAssetId RoomAssetId);
	
	/** Generates the Level */
	UFUNCTION(BlueprintCallable)
	void GenerateLevelLayout(int32 LevelLength);

	/** Translates 2D-coordinates to a linear list */
	UFUNCTION(BlueprintCallable)
	int RoomPos(int32 X, int32 Y, int32 Length);

	/** Check previous rooms relative to this and returns a valid room */
	UFUNCTION(BlueprintCallable)
	UDOGRoomData* GetValidRoom(int32 X, int32 Y, int32 Length);

	/** Checks the room type and stores it in a relevant TArray */
	UFUNCTION(BlueprintCallable)
	void AddToRoomCorrectArray(UDOGRoomData* RoomData);

	/** Should spawn final rooms and ready the teleport to bossroom */
	UFUNCTION(BlueprintCallable)
	void InitateBossSetup();

	UFUNCTION(BlueprintImplementableEvent)
	void OnFinishedGenerating(bool bSuccessSpawn);

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY()
	bool bLevelGenerated = false;

	UPROPERTY()
	bool bPlayerIsSpawned = false;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	
	/** Array of rooms that should be spawned */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="Default")
	TArray<UDOGRoomData*> LevelLayout;

	/** Array of rooms that has been spawned */
	UPROPERTY(BlueprintReadWrite, EditInstanceOnly, Category="Default")
	TArray<ADOGRoom*> LevelLayoutBuilt;

	/** Please add a variable description */
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category="Default")
	TArray<UDOGRoomData*> MerchantRooms;

	/** Please add a variable description */
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category="Default")
	TArray<UDOGRoomData*> GunRooms;

	/** Please add a variable description */
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category="Default")
	TArray<UDOGRoomData*> UpgradeRooms;

	/** Please add a variable description */
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category="Default")
	TArray<UDOGRoomData*> ScrapRooms;

	/** Please add a variable description */
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category="Default")
	TArray<UDOGRoomData*> BossRooms;

	/** Please add a variable description */
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category="Default")
	TArray<UDOGRoomData*> EmptyRooms;

	/** Used to keep track of if assets are loaded */
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category="Default")
	int32 AssetAmount;

	/** Minimum length of a level */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="Default")
	int32 MinLength;

	/** Maximum length of a level */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="Default")
	int32 MaxLength;

	/** Offset for correct room spacing */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="Default")
	int32 RoomSpacingX = 3600;

	/** Offset for correct room spacing */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="Default")
	int32 RoomSpacingY = 2400;
	
	UPROPERTY()
	ADOGRoom* StartingRoom = nullptr;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="Default")
	TSubclassOf<ADOGRoom> StartingRoomClass;

	UPROPERTY(BlueprintReadWrite)
	int GeneratedLength = 0;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, meta=(ExposeOnSpawn="true"))
	ADOGRoom* TutorialStartRoom = nullptr;

	/** The mandatory shop before the boss */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, meta=(ExposeOnSpawn="true"))
	ADOGRoom* PreBossShop = nullptr;

	UPROPERTY()
	AActor* Player = nullptr;

	UPROPERTY()
	bool bHasDoneTutorial = false;
	
};
