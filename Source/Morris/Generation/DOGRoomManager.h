// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "DOGDoor.h"
#include "Morris/AI/DOGEnemyBase.h"
#include "Morris/AI/EnemyTypes/DOGEnemyRanged.h"

#include "DOGRoomManager.generated.h"

class ADOGRoom;
/**
 * 
 */
UCLASS()
class MORRIS_API ADOGRoomManager : public AActor
{
	GENERATED_BODY()
public:
	UPROPERTY(BlueprintReadWrite, EditAnywhere, meta=(ExposeOnSpawn="true"))
	ADOGRoom* RoomUp = nullptr;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, meta=(ExposeOnSpawn="true"))
	ADOGRoom* RoomRight = nullptr;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, meta=(ExposeOnSpawn="true"))
	ADOGRoom* RoomLeft = nullptr;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, meta=(ExposeOnSpawn="true"))
	ADOGRoom* RoomDown = nullptr;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, meta=(ExposeOnSpawn="true"))
	ADOGRoom* RoomCurrent = nullptr;
	
	UPROPERTY()
	ADOGDoor* DoorUp;
	UPROPERTY()
	ADOGDoor* DoorRight;
	UPROPERTY()
	ADOGDoor* DoorLeft;
	UPROPERTY()
	ADOGDoor* DoorBottom;

	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly)
	float SpawnRadius = 600.f;
	
	UPROPERTY(EditDefaultsOnly)
	TArray<ADOGEnemyBase*> Enemies;

	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly)
	int Enemy1Amount = 0;
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly)
	int Enemy2Amount = 0;
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly)
	int Enemy3Amount = 0;
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly)
	int EnemiesPerWave;

	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly)
	TSubclassOf<ADOGEnemyRanged> Enemy1Class = nullptr;
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly)
	TSubclassOf<ADOGEnemyBase> Enemy2Class = nullptr;
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly)
	TSubclassOf<ADOGEnemyBase> Enemy3Class = nullptr;
	
	
	//Chest referense

	UFUNCTION(BlueprintImplementableEvent)
	void OnClearEnemies();

	UFUNCTION(BlueprintImplementableEvent)
	void OnNewWave();

	UFUNCTION(BlueprintImplementableEvent)
	void OnEnterRoom();

	UFUNCTION(BlueprintImplementableEvent)
	void OnExitRoom();

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	bool bShouldCallUpdate = false;

	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly)
	bool bIsBossRoom = false;

	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly)
	float EnemySpawnOffsetZ = 240;
	
protected:
	
	void Update();

	UFUNCTION(BlueprintCallable)
	void NewWave();

	void SpawnEnemy(TSubclassOf<ADOGEnemyBase> Type);
	
	//spawn loot and open doors
	UFUNCTION(BlueprintCallable)
	void RoomCleared();
	
public:
	void ToogleAllDoors(bool Open);
	
	virtual void BeginPlay() override;
	
	//Save which door entered through
	void EnteredRoom();
	
	void ExitedRoom();

	void SetupDoors(ADOGDoor* Top, ADOGDoor* Right, ADOGDoor* Left, ADOGDoor* Bottom);

	UFUNCTION(BlueprintCallable)
	void SpawnRoomSigns();
	
	UFUNCTION(BlueprintCallable)
	bool AllEnemiesDead();

	virtual void Tick(float DeltaSeconds) override;
};
