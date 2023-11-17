// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "DOGEnemySpawner.generated.h"

UCLASS()
class MORRIS_API ADOGEnemySpawner : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	ADOGEnemySpawner();

	void SpawnEnemy();
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;


private:
	// Sets what to spawn - If it doesn't spawn then the spawn point is blocked
	UPROPERTY(EditAnywhere, meta = (AllowPrivateAccess = "true"))
	TSubclassOf<APawn> PawnToSpawn;
	
};
