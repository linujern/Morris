// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "DOGWorldInteractable.h"
#include "GameFramework/Actor.h"
#include "DOGWeaponPickup.generated.h"

UCLASS()
class MORRIS_API ADOGWeaponPickup : public ADOGWorldInteractable
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	ADOGWeaponPickup();

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Weapon")
	TSoftObjectPtr<UDOGWeaponData> WeaponData = nullptr;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Weapon")
	UDOGWeaponData* LoadedWeaponData = nullptr;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	virtual void Interact(AActor* Caller) override;
	
};
