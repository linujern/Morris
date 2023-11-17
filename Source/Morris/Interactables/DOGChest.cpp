// Fill out your copyright notice in the Description page of Project Settings.


#include "DOGChest.h"

#include "DOGStatUpgradePickups.h"
#include "Engine/AssetManager.h"
#include "Engine/StreamableManager.h"
#include "Morris/Data/DOGStatUpgradesData.h"


// Sets default values
ADOGChest::ADOGChest()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
	SetRootComponent(MeshComponent);
	StaticMeshComponent->SetupAttachment(RootComponent);
}

void ADOGChest::Interact(AActor* Caller)
{
	Super::Interact(Caller);
}

void ADOGChest::LoadedAsset(FPrimaryAssetId LoadedId, FVector SpawnLocation)
{
	UAssetManager* Manager = UAssetManager::GetIfValid();
	if (Manager)
	{
		UDOGItemData* ItemData = Cast<UDOGItemData>(Manager->GetPrimaryAssetObject(LoadedId));

		if (ItemData)
		{
			//AActor* NewBot = GetWorld()->SpawnActor<AActor>(MonsterData->MonsterClass, SpawnLocation, FRotator::ZeroRotator);

			OnTriggerChest(ItemData);
			
		}
	}
}

void ADOGChest::EnableChest()
{
	//TODO: Clean
	
	FPrimaryAssetType WeaponType = FPrimaryAssetType("DOGWeaponData");
	TArray<FPrimaryAssetId> WeaponArray;
	UKismetSystemLibrary::GetPrimaryAssetIdList(WeaponType, WeaponArray);

	FPrimaryAssetType StatType = FPrimaryAssetType("DOGStatUpgradesData");
	TArray<FPrimaryAssetId> StatArray;
	UKismetSystemLibrary::GetPrimaryAssetIdList(StatType, StatArray);

	TArray<FPrimaryAssetId> Array;
	if (bWeaponChest)
		Array = WeaponArray;
	else
		Array = StatArray;

	if (Array.Num() == 0) Array = WeaponArray;
	
	FPrimaryAssetId AssetId = Array[FMath::RandRange(0, Array.Num() - 1)];

	// Get the Asset Manager from anywhere
	if (UAssetManager* Manager = UAssetManager::GetIfValid())
	{
		// Monster Id taken from a DataTable
		//FPrimaryAssetId MonsterId = SelectedMonsterRow->MonsterId;

		// Optional "bundles" like "UI"
		TArray<FName> Bundles;

		//TODO: Look if this works also spawn location is wack
		
		// Locations array from omitted part of code (see github)
		FVector SpawnLocation = FVector(0,0,0); 

		// Delegate with parameters we need once the asset had been loaded such as the Id we loaded and the location to spawn at. Will call function 'OnMonsterLoaded' once it's complete.
		FStreamableDelegate Delegate = FStreamableDelegate::CreateUObject(this, &ADOGChest::LoadedAsset, AssetId, SpawnLocation);
    
		// The actual async load request
		Manager->LoadPrimaryAsset(AssetId, Bundles, Delegate);
	}
	//bCanInteract = true;

	
}

void ADOGChest::DisableChest()
{
	bCanInteract = false;
}

AActor* ADOGChest::SpawnItemFromData(UDOGItemData* ItemData, FTransform Transform)
{
	if (ItemData)
	{
		if (ItemData->IsA(UDOGWeaponData::StaticClass()))
		{
			if (!WeaponClass) return nullptr;
			AActor* Item = GetWorld()->SpawnActorDeferred<ADOGWeaponPickup>(WeaponClass, Transform);
			const UDOGWeaponData* WeaponData = Cast<UDOGWeaponData>(ItemData);
			Cast<ADOGWeaponPickup>(Item)->WeaponData = WeaponData;
			Item->FinishSpawning(Transform);
			return Item;
		}

		if (ItemData->IsA(UDOGStatUpgradesData::StaticClass()))
		{
			if (!StatClass) return nullptr; 
			AActor* Item = GetWorld()->SpawnActorDeferred<ADOGStatUpgradePickups>(StatClass, Transform);
			const UDOGStatUpgradesData* StatUpgradesData = Cast<UDOGStatUpgradesData>(ItemData);
			Cast<ADOGStatUpgradePickups>(Item)->StatUpgradesData = StatUpgradesData;
			Item->FinishSpawning(Transform);
			return Item;
		}
	}
	
	return nullptr;
}

// Called when the game starts or when spawned
void ADOGChest::BeginPlay()
{
	Super::BeginPlay();
	bCanInteract = false;
}

// Called every frame
void ADOGChest::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

