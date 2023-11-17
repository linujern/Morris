// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "DOGScrapMetal.generated.h"

UCLASS()
class MORRIS_API ADOGScrapMetal : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	ADOGScrapMetal();

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	UStaticMeshComponent* MeshComponent;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(ExposeOnSpawn="true"))
	float Value = 1.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(ExposeOnSpawn="true"))
	bool bIsGolden = false;
	
	UPROPERTY()
	bool bIsBeingCollected = false;
	
	void TryCollect(AActor* Caller);

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;
};
