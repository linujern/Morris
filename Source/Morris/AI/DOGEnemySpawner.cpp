// Fill out your copyright notice in the Description page of Project Settings.


#include "DOGEnemySpawner.h"

#include "DOGEnemyBase.h"

// Sets default values
ADOGEnemySpawner::ADOGEnemySpawner()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
}

// Called when the game starts or when spawned
void ADOGEnemySpawner::BeginPlay()
{
	Super::BeginPlay();
	SpawnEnemy();
}

void ADOGEnemySpawner::SpawnEnemy()
{
	// Print message on screen
	GEngine->AddOnScreenDebugMessage(-1, 5, FColor::White, TEXT("Enemy Spawned"));
	// Spawns enemy - Pawn to Spawn is selected in inspector
	GetWorld()->SpawnActor<ADOGEnemyBase>(PawnToSpawn, GetTransform());
}

