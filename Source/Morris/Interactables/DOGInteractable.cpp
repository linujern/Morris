// Fill out your copyright notice in the Description page of Project Settings.


#include "DOGInteractable.h"


// Sets default values
ADOGInteractable::ADOGInteractable()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	MeshComponent = CreateDefaultSubobject<USkeletalMeshComponent>("Mesh");
	RootComponent = MeshComponent;

}

// Called when the game starts or when spawned
void ADOGInteractable::BeginPlay()
{
	Super::BeginPlay();
	
}

void ADOGInteractable::Interact(AActor* Caller)
{
	//
}

// Called every frame
void ADOGInteractable::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

