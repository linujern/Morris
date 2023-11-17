// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "DOGInteractable.h"
#include "GameFramework/Actor.h"
#include "Morris/Data/DOGPowerupData.h"
#include "Morris/Player/DOGPlayer.h"
#include "DOGPowerup.generated.h"

UCLASS()
class MORRIS_API ADOGPowerup : public ADOGInteractable
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	ADOGPowerup();

	
	virtual void Interact(AActor* Caller) override;

	UFUNCTION()
	void HealPower();

	UFUNCTION()
	void EndTimePower();
	
	//UFUNCTION(BlueprintCallable)
	//bool CanUse(AActor* Caller);

	UFUNCTION(BlueprintCallable)
	void StartDuration();

	UFUNCTION(BlueprintCallable)
	void EndDuration();
	
	UFUNCTION(BlueprintCallable)
	void StartCooldown();

	UFUNCTION(BlueprintNativeEvent)
	void OnDurationStart();
	
	UFUNCTION()
	void CancelCooldown();
	
	UFUNCTION(BlueprintNativeEvent)
	void OnDurationEnd();
	
	UFUNCTION(BlueprintCallable)
	void EndCooldown();
	
	UFUNCTION(BlueprintNativeEvent)
	void OnStartCooldown();
	
	UFUNCTION(BlueprintNativeEvent)
	void OnEndCooldown();

	UFUNCTION(BlueprintImplementableEvent)
	void OnPowerBurst();

	UFUNCTION(BlueprintImplementableEvent)
	void OnPowerTimer();

	UFUNCTION()
	void PowerTimer();
	UFUNCTION()
	void PowerBurst();
	
	UFUNCTION(BlueprintImplementableEvent)
	void OnTimePower();
	UFUNCTION(BlueprintImplementableEvent)
	void OnHealPower();

	
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Power Up")
	UDOGPowerupData* PowerupData = nullptr;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	bool bOnDuration = false;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	bool bOnCooldown = false;

	UPROPERTY()
	FTimerHandle PowerHandle;

	UPROPERTY()
	FTimerHandle EffectHandle;

	UPROPERTY()
	FTimerHandle CooldownHandle;

	UPROPERTY()
	FTimerHandle DurationHandle;

	UPROPERTY()
	ADOGPlayer* Player = nullptr;

	//UPROPERTY()
	//UNiagaraComponent* PowerParticleSystem = nullptr;
};
