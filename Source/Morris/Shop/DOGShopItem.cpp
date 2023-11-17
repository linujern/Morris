// Fill out your copyright notice in the Description page of Project Settings.


#include "DOGShopItem.h"

#include "AssetRegistry/AssetRegistryModule.h"
#include "AssetRegistry/IAssetRegistry.h"
#include "Engine/AssetManager.h"
#include "Kismet/KismetMathLibrary.h"
#include "Morris/Data/DOGStatUpgradesData.h"
#include "Morris/Interactables/DOGPowerPickup.h"
#include "Morris/Interactables/DOGStatUpgradePickups.h"
#include "Morris/Interactables/DOGWeaponPickup.h"
#include "Morris/Player/DOGPlayer.h"


// Sets default values
ADOGShopItem::ADOGShopItem()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	RootComponent = StaticMeshComponent;
	MeshComponent->SetupAttachment(RootComponent);

	
}
// Called when the game starts or when spawned
void ADOGShopItem::BeginPlay()
{
	Super::BeginPlay();

	IAssetRegistry& AssetRegistry = FAssetRegistryModule::GetRegistry();
	
	FARFilter ARFilter;
	ARFilter.bRecursiveClasses = true;
	ARFilter.bRecursivePaths = true;
	ARFilter.ClassPaths.Add(UDOGItemData::StaticClass()->GetClassPathName());
	ARFilter.PackagePaths.Add(TEXT("/Game/Data/Items"));

	// Exclude classes from item search 
	switch (ShopType)
	{
	case ALLSHOP:
		break;
	case WEAPONSHOP:
		ARFilter.RecursiveClassPathsExclusionSet.Add(UDOGStatUpgradesData::StaticClass()->GetClassPathName());
		ARFilter.RecursiveClassPathsExclusionSet.Add(UDOGPowerupData::StaticClass()->GetClassPathName());
		break;
	case POWERSHOP:
		ARFilter.RecursiveClassPathsExclusionSet.Add(UDOGWeaponData::StaticClass()->GetClassPathName());
		ARFilter.RecursiveClassPathsExclusionSet.Add(UDOGStatUpgradesData::StaticClass()->GetClassPathName());
		break;
	case STATSHOP:
		ARFilter.RecursiveClassPathsExclusionSet.Add(UDOGWeaponData::StaticClass()->GetClassPathName());
		ARFilter.RecursiveClassPathsExclusionSet.Add(UDOGPowerupData::StaticClass()->GetClassPathName());
		break;
	case WEAPONPOWERSHOP:
		ARFilter.RecursiveClassPathsExclusionSet.Add(UDOGStatUpgradesData::StaticClass()->GetClassPathName());
		break;
	case WEAPONSTATSHOP:
		ARFilter.RecursiveClassPathsExclusionSet.Add(UDOGPowerupData::StaticClass()->GetClassPathName());
		break;
	case POWERSTATSHOP:
		ARFilter.RecursiveClassPathsExclusionSet.Add(UDOGWeaponData::StaticClass()->GetClassPathName());
		break;
	default:
		break;
	}
	
	TArray<FSoftObjectPath> AssetList;
	TArray<FAssetData> Assets;
	
	AssetRegistry.GetAssets(ARFilter, Assets);
	AssetList.Reserve(Assets.Num());
	
	for (int32 i = 0; i < Assets.Num(); i++)
	{
		const FAssetData& AssetData = Assets[i];
	
		//int32 TriangleCount;
		//if (AssetData.GetTagValue<int32>("Triangles", TriangleCount) && TriangleCount <= 0 )
		//{ 
			AssetList.Add(AssetData.ToSoftObjectPath());	
		//}
	}
	
	AssetList.Shrink();
	
	UAssetManager::Get().LoadAssetList(AssetList, FStreamableDelegate::CreateLambda([AssetList, this]
	{
		const int RandomNumberInAssetList = UKismetMathLibrary::RandomInteger(AssetList.Num());
	
		if (IsValid(Cast<UDOGItemData>(AssetList[RandomNumberInAssetList].ResolveObject())))
		{
			UDOGItemData* NewItemData = Cast<UDOGItemData>(AssetList[RandomNumberInAssetList].ResolveObject());
			GetItemData(NewItemData);
			//UKismetSystemLibrary::PrintString(GetWorld(), ItemData->ItemName.ToString());	
		}
	}));
	
	//TODO: Randomize item -- Completed i think needs to be checked
	
}

// Called every frame
void ADOGShopItem::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ADOGShopItem::GetItemData(UDOGItemData* Data)
{
	if (Data)
	{
		//UKismetSystemLibrary::PrintString(GetWorld(), "CoolItemData");	

		ItemData = Data;
		ItemName = Data->ItemName;
		ItemColor = Data->ItemColor;
		ItemPrice = FMath::RandRange(Data->ShopPrice.Min, Data->ShopPrice.Max);

		 
	
		StaticMeshComponent->SetStaticMesh(Data->ShopMesh);
	
		switch (Data->ShopItemType)
		{
		case WEAPON:
			{
				if (UDOGWeaponData* WeaponData = Cast<UDOGWeaponData>(Data))
				//if(WeaponData->GunMesh != nullptr)
				MeshComponent->SetSkeletalMesh(WeaponData->GunMesh);
				else
				{
					UKismetSystemLibrary::PrintString(GetWorld(), "Error Shop item ShopItemType cast return null");	
				}
			}
			break;
		case POWERUP:
			{
				UDOGPowerupData* PowerupData = Cast<UDOGPowerupData>(Data);
				if(PowerupData->PowerupMesh != nullptr)
					MeshComponent->SetSkeletalMesh((PowerupData->PowerupMesh));
				else if (PowerupData->PowerupMesh != nullptr)
					MeshComponent->SetSkeletalMesh(PowerupData->PowerupMesh);
			}
			break;
		case STAT:
			{
				UDOGStatUpgradesData* StatUpgradesData = Cast<UDOGStatUpgradesData>(Data);
				if(StatUpgradesData->ShopMesh != nullptr)
					StaticMeshComponent->SetStaticMesh((StatUpgradesData->ShopMesh));
			}
			break;
		default:
			{
				UE_LOG(LogTemp, Error, TEXT("Get Item Data shop item type NULL - DOGShopItem::GetItemData 154"))
			}
			break;
		}

	}
	UE_LOG(LogTemp, Error, TEXT("Item Data NULL - DOGShopItem::GetItemData 160"))
}

void ADOGShopItem::Interact(AActor* Caller)
{
	Super::Interact(Caller);
	ADOGPlayer* Player = Cast<ADOGPlayer>(Caller);
	if(Player->ScrapMetal >= ItemPrice)
	{
		if (PurchaseSound)
			UGameplayStatics::PlaySoundAtLocation(GetWorld(), PurchaseSound, GetActorLocation());
		const int Rand = FMath::RandRange(1, 5);
		if (PurchaseVoicelines && Rand != 1)
			UGameplayStatics::PlaySoundAtLocation(GetWorld(), PurchaseVoicelines, GetActorLocation());

		Player->ScrapMetal = Player->ScrapMetal - ItemPrice;
		Player->SaveScrapMetal();
		//UKismetSystemLibrary::PrintString(GetWorld(), "Purchased");
		switch (ItemData->ShopItemType)
		{
		case WEAPON:
			{
				const FTransform SpawnLocAndRotation = GetActorTransform();
				ADOGWeaponPickup* Weapon = GetWorld()->SpawnActorDeferred<ADOGWeaponPickup>(ADOGWeaponPickup::StaticClass(), SpawnLocAndRotation);
				UDOGWeaponData* WeaponData = Cast<UDOGWeaponData>(ItemData);
				Weapon->LoadedWeaponData = WeaponData;
				Weapon->FinishSpawning(SpawnLocAndRotation);
				
				Weapon->Interact(Player);

				Destroy();
			}
		break;
		case POWERUP:
			{
				const FTransform SpawnLocAndRotation = GetActorTransform();
				ADOGPowerPickup* Powerup = GetWorld()->SpawnActorDeferred<ADOGPowerPickup>(ADOGPowerPickup::StaticClass(), SpawnLocAndRotation);
				UDOGPowerupData* PowerupData = Cast<UDOGPowerupData>(ItemData);
				Powerup->PowerPickupData = PowerupData;
				Powerup->FinishSpawning(SpawnLocAndRotation);

				Powerup->Interact(Player);

				Destroy();
			}
		break;
		case STAT:
			{
				const FTransform SpawnLocAndRotation = GetActorTransform();
				ADOGStatUpgradePickups* Statpickup = GetWorld()->SpawnActorDeferred<ADOGStatUpgradePickups>(ADOGStatUpgradePickups::StaticClass(), SpawnLocAndRotation);
				UDOGStatUpgradesData* StatData = Cast<UDOGStatUpgradesData>(ItemData);
				Statpickup->StatUpgradesData = StatData;
				Statpickup->FinishSpawning(SpawnLocAndRotation);

				Statpickup->Interact(Player);

				Destroy();
			}
		break;

			default:
				break;
		}
	}
	else
	{
		if (PurchaseNoMoneySound)
			UGameplayStatics::PlaySoundAtLocation(GetWorld(), PurchaseNoMoneySound, GetActorLocation());
	}
}


