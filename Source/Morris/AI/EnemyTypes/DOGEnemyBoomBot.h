// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "Morris/AI/DOGEnemyBase.h"
#include "DOGEnemyBoomBot.generated.h"

UCLASS()
class MORRIS_API ADOGEnemyBoomBot : public ADOGEnemyBase
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	ADOGEnemyBoomBot();

	void Explode();

	UFUNCTION(BlueprintImplementableEvent)
	void OnExplosion();

	// Amount of damage to the player
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Enemy|Stats|Boombot")
	float Damage = 40.0f;
	
	// How big the damage zone for the explosion is - if 0 defaults to 50
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Enemy|Stats|Boombot")
	float ExplosionRadius = 50.0f;

	
	
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;


};
