// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "DOGItemData.h"
#include "NiagaraSystem.h"
#include "DOGPowerupData.generated.h"

/**
 * 
 */
UCLASS()
class MORRIS_API UDOGPowerupData : public UDOGItemData
{
	GENERATED_BODY()

public:

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Power Up",
		meta=(ToolTip="Power type, VERY IMPORTANT!"))
	TEnumAsByte<EPowerUpType> PowerUpType = HEALTH;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Power Up",
		meta=(ToolTip="How strong the start effect of the ability is"))
	float BurstEffect = 10.f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Power Up",
		meta=(ToolTip="How strong the timer effect of the ability is"))
	float TimerEffect = 2.f;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Power Up",
		meta=(ToolTip="Affects the time between activations during the duration, Cant be bigger than Duration!"))
	float PowerTimerRate = 1.f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Power Up",
		meta=(ToolTip="Lenght that the ability is in effect"))
	float PowerDuration = 10.f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Power Up",
		meta=(ToolTip="Lenght of the Cooldown"))
	float PowerCooldown = 10.f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Power Up",
		meta=(ToolTip="Cost of activating a ability"))
	float BurstScrapCost = 0.f;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Power Up",
		meta=(ToolTip="Cost of activating the timer of a ability"))
	float TimerScrapCost = 0.f;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Power Up",
		meta=(ToolTip="The mesh that this powerup will use"))
	USkeletalMesh* PowerupMesh = nullptr;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Power Up")
	FLinearColor PowerUpColor = FLinearColor::Red;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Power Up")
	UMaterialParameterCollection* MaterialParameterCollection = nullptr;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Power Up")
	UNiagaraSystem* ParticleSystem = nullptr;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Power Up",
	meta=(ToolTip="The sound that the power will play on use"))
	USoundBase* UseSound = nullptr;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Power Up",
	meta=(ToolTip="The sound that the power will play on timer effect"))
	USoundBase* TimerSound = nullptr;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Power Up",
	meta=(ToolTip="The Sound it will make when the ability is completed"))
	USoundBase* StopUseSound = nullptr;

};
