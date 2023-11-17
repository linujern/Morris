// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "DOGInteractable.h"
#include "GameFramework/Actor.h"
#include "Morris/Data/DOGWeaponData.h"
#include "DOGWeapon.generated.h"

UCLASS()
class MORRIS_API ADOGWeapon : public ADOGInteractable
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	ADOGWeapon();

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Weapon")
	UDOGWeaponData* WeaponData = nullptr;

	UPROPERTY()
	bool bOnFireCooldown = false;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	bool bIsReloading = false;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	bool bSpeedBoost = false;

	UPROPERTY()
	AActor* Target = nullptr;

	UPROPERTY()
	FTimerHandle FireCooldownHandle;

	UPROPERTY()
	FTimerHandle ReloadTimerHandle;

	//DATA
	UPROPERTY()
	float CurrentAmmo;
	
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	virtual void Interact(AActor* Caller) override;

private:
	void AimAssist();

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION(BlueprintCallable)
	void StartReload();

	UFUNCTION(BlueprintCallable)
	void EndReload();

	UFUNCTION()
	void ResetFirecooldown();

	UFUNCTION(BlueprintNativeEvent)
	void OnStartReload();

	UFUNCTION(BlueprintNativeEvent)
	void OnEndReload();


	UFUNCTION(BlueprintCallable)
	bool CanShoot(AActor* Caller);

	UFUNCTION()
	bool TryCancelReload();
	
};
