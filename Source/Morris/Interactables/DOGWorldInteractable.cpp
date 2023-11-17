// Fill out your copyright notice in the Description page of Project Settings.


#include "DOGWorldInteractable.h"

#include "DOGPowerPickup.h"
#include "DOGStatUpgradePickups.h"
#include "DOGWeaponPickup.h"
#include "Morris/Shop/DOGShopItem.h"


// Sets default values
ADOGWorldInteractable::ADOGWorldInteractable()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	MeshComponent = CreateDefaultSubobject<USkeletalMeshComponent>("Mesh");
	RootComponent = MeshComponent;

	MeshComponent->SetCollisionProfileName(TEXT("BlockAllDynamic"));
	
	StaticMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>("StaticMesh");
}

void ADOGWorldInteractable::ToggleOutline(bool bIsVisible)
{
	// Weapons
	if (!bCanInteract)
	{
		MeshComponent->SetRenderCustomDepth(false);
		MeshComponent->SetCustomDepthStencilValue(1.f);
		return;			
	}
	MeshComponent->SetRenderCustomDepth(bIsVisible);
	MeshComponent->SetCustomDepthStencilValue(1.f);

	// Items
	if (!bCanInteract)
	{
		StaticMeshComponent->SetRenderCustomDepth(false);
		StaticMeshComponent->SetCustomDepthStencilValue(1.f);
		return;
	}
	StaticMeshComponent->SetRenderCustomDepth(bIsVisible);
	StaticMeshComponent->SetCustomDepthStencilValue(1.f);
}

void ADOGWorldInteractable::SetItemUI()
{
	UTexture2D* BackgroundImage = nullptr;
	UTexture2D* IconImage = nullptr;
	
	FText Name = FText::FromString("ITEM NAME");
	FText Type = FText::FromString("");
	FText Price = FText::FromString("FREE");
	FText Details = FText::FromString("");

	float Damage = 0.0f;
	float Firerate = 0.0f;
	float Range = 0.0f;
	float Multishot = 0.0f;
	
	if (this->IsA(ADOGShopItem::StaticClass()))
	{
		const ADOGShopItem* ShopItem = Cast<ADOGShopItem>(this);
		Name = FText::FromString(ShopItem->ItemName.ToString());
		BackgroundImage = ShopItem->ItemData->ItemBackgroundImage;
		IconImage = ShopItem->ItemData->ItemIconImage;
		
		//Type = FText::FromString(ShopItem->ItemData->ShopItemType);
		Price = FText::FromString("COST: " + FString::FromInt(floor(ShopItem->ItemPrice)));
		//Details = FText::FromString(ShopItem->ItemData->ItemDescription);

		if (ShopItem->ItemData->ShopItemType == WEAPON)
		{
			if (UDOGWeaponData* Data = Cast<UDOGWeaponData>(ShopItem->ItemData))
			{
				Damage = Data->BaseDamage / 100.f;
				Firerate = ((Data->FireRate - 1) * -1);
				Range = Data->Range / 3000.f;
				if (Data->bEnableMultishot)
					Multishot = Data->MultiShots / 10.f;
				Details = FText::FromString(Data->ItemDescription);
			}
		}
		else
		{
			HudWidget->DamageBar->SetVisibility(ESlateVisibility::Collapsed);
			HudWidget->FireRateBar->SetVisibility(ESlateVisibility::Collapsed);
			HudWidget->RangeBar->SetVisibility(ESlateVisibility::Collapsed);
			HudWidget->MultiShotBar->SetVisibility(ESlateVisibility::Collapsed);
			Details = FText::FromString(ShopItem->ItemData->ItemDescription);
		}

		
	}
	else if (this->IsA(ADOGWeaponPickup::StaticClass()))
	{
		const ADOGWeaponPickup* Weapon = Cast<ADOGWeaponPickup>(this);
		BackgroundImage = Weapon->WeaponData->ItemBackgroundImage;
		IconImage = Weapon->WeaponData->ItemIconImage;
		
		Name = FText::FromString(Weapon->WeaponData->ItemName.ToString());
		//Type = FText::FromString(ShopItem->ItemData->ShopItemType);
		//Price = FText::FromString("COST: " + FString::FromInt(floor(Weapon->WeaponData)));
		Details = FText::FromString(Weapon->WeaponData->ItemDescription);
		
		
		Damage = Weapon->WeaponData->BaseDamage / 100.f;
		Firerate = ((Weapon->WeaponData->FireRate - 1) * -1);
		Range = Weapon->WeaponData->Range / 3000.f;
		if (Weapon->WeaponData->bEnableMultishot)
			Multishot = Weapon->WeaponData->MultiShots / 10.f;
	}
	else if (this->IsA(ADOGPowerPickup::StaticClass()))
	{

		const ADOGPowerPickup* Power = Cast<ADOGPowerPickup>(this);
		Name = FText::FromString(Power->PowerPickupData->ItemName.ToString());
		HudWidget->DamageBar->SetVisibility(ESlateVisibility::Collapsed);
		HudWidget->FireRateBar->SetVisibility(ESlateVisibility::Collapsed);
		HudWidget->RangeBar->SetVisibility(ESlateVisibility::Collapsed);
		HudWidget->MultiShotBar->SetVisibility(ESlateVisibility::Collapsed);
		Details = FText::FromString(Power->PowerPickupData->ItemDescription);
	}
	else if (this->IsA(ADOGStatUpgradePickups::StaticClass()))
	{

		const ADOGStatUpgradePickups* Stats = Cast<ADOGStatUpgradePickups>(this);
		Name = FText::FromString(Stats->StatUpgradesData->ItemName.ToString());
		HudWidget->DamageBar->SetVisibility(ESlateVisibility::Collapsed);
		HudWidget->FireRateBar->SetVisibility(ESlateVisibility::Collapsed);
		HudWidget->RangeBar->SetVisibility(ESlateVisibility::Collapsed);
		HudWidget->MultiShotBar->SetVisibility(ESlateVisibility::Collapsed);
		Details = FText::FromString(Stats->StatUpgradesData->ItemDescription);
	}

	if (BackgroundImage)
		HudWidget->BackgroundImage->SetBrushFromTexture(BackgroundImage) ;
	if (IconImage)
		HudWidget->IconImage->SetBrushFromTexture(IconImage);
	
	HudWidget->NameText->SetText(Name);
	HudWidget->TypeText->SetText(Type);
	HudWidget->CostText->SetText(Price);
	HudWidget->DetailText->SetText(Details);

	HudWidget->DamageBar->SetPercent(Damage);
	HudWidget->FireRateBar->SetPercent(Firerate + 0.05f);
	HudWidget->RangeBar->SetPercent(Range);
	HudWidget->MultiShotBar->SetPercent(Multishot);
}

ADOGWorldInteractable* ADOGWorldInteractable::InRangeOfItem()
{
	if (HudWidget && !HudWidget->IsInViewport())
	{
		//UKismetSystemLibrary::PrintString(GetWorld(), "In Range");

		HudWidget->AddToViewport();
		//WidgetComponent->Activate(true);
	}
	return this;
}

ADOGWorldInteractable* ADOGWorldInteractable::NotInRangeOfItem()
{
	//UKismetSystemLibrary::PrintString(GetWorld(), "Out of Range");
	if (HudWidget && HudWidget->IsInViewport())
	{
		//UKismetSystemLibrary::PrintString(GetWorld(), "Out of Range the good part");
		HudWidget->RemoveFromParent();
		//WidgetComponent->Activate(false);
	}	
	return nullptr;
}

// Called when the game starts or when spawned
void ADOGWorldInteractable::BeginPlay()
{
	Super::BeginPlay();

	// UI
	
	if(HudWidgetClass != nullptr)
	{
		UWidget* Widget = CreateWidget(GetWorld(), HudWidgetClass);
		HudWidget = Cast<UDOGItemUI>(Widget);
		// TODO: Complete
		// Set Image?

		FText Price = FText::FromString("FREE");
		if (ADOGShopItem* ShopItem = Cast<ADOGShopItem>(this))
			Price = FText::FromString("COST: " + FString::FromInt(floor(ShopItem->ItemPrice)));
		HudWidget->CostText->SetText(Price);
		
		HudWidget->ItemReference = this;
		
	}
}

// Called every frame
void ADOGWorldInteractable::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ADOGWorldInteractable::Interact(AActor* Caller)
{
	//
	if (!bCanInteract)
		return;
	
}

