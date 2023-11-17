// Fill out your copyright notice in the Description page of Project Settings.


#include "DOGEnemyBoomBot.h"

#include "Kismet/KismetSystemLibrary.h"


// Sets default values
ADOGEnemyBoomBot::ADOGEnemyBoomBot()
{
	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
}

void ADOGEnemyBoomBot::Explode()
{
	//UKismetSystemLibrary::PrintString(GetWorld(), "Explode");

	OnExplosion();
	
	Destroy();
}

// Called when the game starts or when spawned
void ADOGEnemyBoomBot::BeginPlay()
{
	Super::BeginPlay();
	
}



