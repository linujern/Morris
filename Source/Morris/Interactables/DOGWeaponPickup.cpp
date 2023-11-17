// Fill out your copyright notice in the Description page of Project Settings.


#include "DOGWeaponPickup.h"

#include "DOGWeapon.h"
#include "Morris/Player/DOGPlayer.h"


// Sets default values
ADOGWeaponPickup::ADOGWeaponPickup()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
}

// Called when the game starts or when spawned
void ADOGWeaponPickup::BeginPlay()
{
	Super::BeginPlay();
	if(WeaponData.IsNull())
	{
		UE_LOG(LogTemp, Error, TEXT("Error loading gun data. Data is null. DOGWeaponPickup Begin play - 23"));
		return;
	}
	const FSoftObjectPath WeaponDataPath = WeaponData.ToSoftObjectPath();
	LoadedWeaponData = Cast<UDOGWeaponData>(WeaponDataPath.ResolveObject());
	if (LoadedWeaponData == nullptr)
	{
		LoadedWeaponData = CastChecked<UDOGWeaponData>(WeaponDataPath.TryLoad());
	}
	UE_LOG(LogTemp,	Warning, TEXT("Loaded weapon data."));
	MeshComponent->SetSkeletalMesh(LoadedWeaponData->GunMesh);
	MeshComponent->SetRenderCustomDepth(false);
	MeshComponent->SetCustomDepthStencilValue(0.f);
}

// Called every frame
void ADOGWeaponPickup::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ADOGWeaponPickup::Interact(AActor* Caller)
{
	Super::Interact(Caller);

	const FTransform SpawnLocAndRotation = GetActorTransform();
	ADOGWeapon* Weapon = GetWorld()->SpawnActorDeferred<ADOGWeapon>(ADOGWeapon::StaticClass(), SpawnLocAndRotation);
	Weapon->WeaponData = LoadedWeaponData;
	Weapon->FinishSpawning(SpawnLocAndRotation);
	
	ADOGPlayer* Player = Cast<ADOGPlayer>(Caller);

	if(!IsValid(Player->SecondaryEquipment))
	{
		Player->SwapHands();
	}

	if(IsValid(Player->PrimaryEquipment) && IsValid(Player->SecondaryEquipment))
	{
		Player->TryDropHand();
	}

	Weapon->AttachToComponent(Cast<ADOGPlayer>(Caller)->HandComponent, FAttachmentTransformRules(EAttachmentRule::SnapToTarget, false));

	Player->PrimaryEquipment = Weapon;

	Destroy();
	
}

