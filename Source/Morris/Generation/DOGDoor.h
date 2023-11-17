// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "DOGDoor.generated.h"

UCLASS(Abstract, Blueprintable)
class MORRIS_API ADOGDoor : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	ADOGDoor();

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	bool bIsOpen;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	bool bIsEntrance;

	UFUNCTION(BlueprintImplementableEvent)
	void OnDoorOpen();
	UFUNCTION(BlueprintImplementableEvent)
	void OnDoorClose();
	UFUNCTION(BlueprintImplementableEvent)
	void OnFinishSpawning();

	
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;
};
