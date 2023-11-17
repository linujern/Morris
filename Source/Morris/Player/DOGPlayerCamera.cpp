// Fill out your copyright notice in the Description page of Project Settings.


#include "DOGPlayerCamera.h"


// Sets default values
ADOGPlayerCamera::ADOGPlayerCamera()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	SpringArmComponent = CreateDefaultSubobject<USpringArmComponent>("SpringArm");
	RootComponent = SpringArmComponent;

	CameraComponent = CreateDefaultSubobject<UCameraComponent>("Mesh");
	CameraComponent->SetupAttachment(SpringArmComponent);
	//CameraComponent->RegisterComponent();
}

// Called when the game starts or when spawned
void ADOGPlayerCamera::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ADOGPlayerCamera::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if(Player)
	{
		FVector ToLoc = Player->GetActorLocation() + Player->GetActorForwardVector() * -160.f;
		SetActorLocation(FVector(ToLoc.X, ToLoc.Y, ToLoc.Z+240.f));
	}
}

