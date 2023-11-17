// Fill out your copyright notice in the Description page of Project Settings.


#include "DOGStatUpgradePickups.h"

#include "Morris/Player/DOGPlayer.h"

ADOGStatUpgradePickups::ADOGStatUpgradePickups()
{
	RootComponent = StaticMeshComponent;
	MeshComponent->SetupAttachment(RootComponent);
}

void ADOGStatUpgradePickups::BeginPlay()
{
	Super::BeginPlay();

	if (StatUpgradesData.IsNull())
	{
		UE_LOG(LogTemp, Error, TEXT("Error loading gun data. Data is null. DOGWorldInteractable#BeginPlay"));
		return;
	}
		// Get Random Data 
	const FSoftObjectPath StatsUpgradeDataPath = StatUpgradesData.ToSoftObjectPath();
	LoadedStatsData = Cast<UDOGStatUpgradesData>(StatsUpgradeDataPath.ResolveObject());
	if (!IsValid(LoadedStatsData))
	{
		LoadedStatsData = CastChecked<UDOGStatUpgradesData>(StatsUpgradeDataPath.TryLoad());
	}
	
	StaticMeshComponent->SetStaticMesh(StatUpgradesData->ShopMesh);
	StaticMeshComponent->SetRenderCustomDepth(false);
	StaticMeshComponent->SetCustomDepthStencilValue(1.f);
}

void ADOGStatUpgradePickups::Interact(AActor* Caller)
{
	Super::Interact(Caller);

	ADOGPlayer* Player = Cast<ADOGPlayer>(Caller);
	
	// Give stats to player
	switch (StatUpgradesData->StatType)
	{
		default:
			UE_LOG(LogTemp, Error, TEXT("Stat Type defaulted - StatUpgradePickups"))
			break;

		case HEALTHUPGRADE:
			Player->MaxHealth += StatUpgradesData->StatIncrease;
			Player->ApplyDamage(-StatUpgradesData->StatIncrease);
			break;
			
		case DAMAGEUPGRADE:
			Player->DamageMultiplier += StatUpgradesData->StatIncrease;
			break;
			
		//case SPEEDUPGRADE:
			//TODO: Give player speed variable and implement speed / or Remove it
		//	break;
			
		case DASHUPGRADE:
			Player->MaxDash += 1.f;
			Player->CurrentDash += 1.f;
			break;
	}

	Destroy();
}
