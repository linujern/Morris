// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "DOGInteractable.generated.h"

UCLASS()
class MORRIS_API ADOGInteractable : public AActor
{
	GENERATED_BODY()

public:
	/** Skeletal mesh */
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	USkeletalMeshComponent* MeshComponent;
	
	// Sets default values for this actor's properties
	ADOGInteractable();

	virtual void Interact(AActor* Caller);
	
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;
};
