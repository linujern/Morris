// Fill out your copyright notice in the Description page of Project Settings.


#include "DOGScrapMetal.h"

#include "Morris/Player/DOGPlayer.h"


// Sets default values
ADOGScrapMetal::ADOGScrapMetal()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	MeshComponent = CreateDefaultSubobject<UStaticMeshComponent>("Mesh");
	RootComponent = MeshComponent;

	MeshComponent->SetCollisionProfileName(FName("BlockAllDynamic"));
}

void ADOGScrapMetal::TryCollect(AActor* Caller)
{
	if(!bIsBeingCollected)
	{
		bIsBeingCollected = true;
		//TODO: Collect properly
		ADOGPlayer* Player = Cast<ADOGPlayer>(Caller);
		Player->ScrapMetal+=Value;
		Player->SaveScrapMetal();
		//UKismetSystemLibrary::PrintString(GetWorld(), "Scrap: " + FString::FromInt(floor(Player->ScrapMetal)));
		
		Destroy();
	}
}

// Called when the game starts or when spawned
void ADOGScrapMetal::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ADOGScrapMetal::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

