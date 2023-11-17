// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/FloatingPawnMovement.h"
#include "DOGMovementComponent.generated.h"


UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class MORRIS_API UDOGMovementComponent : public UFloatingPawnMovement
{
	GENERATED_BODY()

public:
	// Sets default values for this component's properties
	UDOGMovementComponent();

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	float GravityMultiplier = 1.f;
	
protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType,
	                           FActorComponentTickFunction* ThisTickFunction) override;
};
