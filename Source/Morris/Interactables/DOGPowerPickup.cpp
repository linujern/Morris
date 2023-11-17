// Fill out your copyright notice in the Description page of Project Settings.


#include "DOGPowerPickup.h"

#include "DOGPowerup.h"
#include "Morris/Player/DOGPlayer.h"

ADOGPowerPickup::ADOGPowerPickup()
{
	RootComponent = StaticMeshComponent;
	MeshComponent->SetupAttachment(RootComponent);
}

void ADOGPowerPickup::BeginPlay()
{
	Super::BeginPlay();
	if (PowerPickupData.IsNull())
	{
		UE_LOG(LogTemp, Error, TEXT("Error loading Power data. Data is null. DOGPowePickup#BeginPlay"));
		return;
	}
	// Get Random Data 
	const FSoftObjectPath StatsUpgradeDataPath = PowerPickupData.ToSoftObjectPath();
	LoadedPowerPickupData = Cast<UDOGPowerupData>(StatsUpgradeDataPath.ResolveObject());
	if (!IsValid(LoadedPowerPickupData))
	{
		LoadedPowerPickupData = CastChecked<UDOGPowerupData>(StatsUpgradeDataPath.TryLoad());
	}
	
	StaticMeshComponent->SetStaticMesh(PowerPickupData->ShopMesh);
	StaticMeshComponent->SetRenderCustomDepth(false);
	StaticMeshComponent->SetCustomDepthStencilValue(1.f);

}

void ADOGPowerPickup::Interact(AActor* Caller)
{
	Super::Interact(Caller);

	
	const FTransform SpawnLocAndRotation = GetActorTransform();
	ADOGPowerup* Powerup = GetWorld()->SpawnActorDeferred<ADOGPowerup>(ADOGPowerup::StaticClass(), SpawnLocAndRotation);
	Powerup->PowerupData = LoadedPowerPickupData;
	Powerup->FinishSpawning(SpawnLocAndRotation);
	
	ADOGPlayer* Player = Cast<ADOGPlayer>(Caller);

	if(!IsValid(Player->SecondaryEquipment))
	{
		Player->SwapHands();
	}

	if(IsValid(Player->PrimaryEquipment) && IsValid(Player->SecondaryEquipment))
	{
		Player->TryDropHand();
	}

	Powerup->AttachToComponent(Cast<ADOGPlayer>(Caller)->HandComponent, FAttachmentTransformRules(EAttachmentRule::SnapToTarget, false));

	Player->PrimaryEquipment = Powerup;

	Destroy();

}
