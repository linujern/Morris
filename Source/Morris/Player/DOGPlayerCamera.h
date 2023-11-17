// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/Actor.h"
#include "GameFramework/SpringArmComponent.h"
#include "DOGPlayerCamera.generated.h"

UCLASS()
class MORRIS_API ADOGPlayerCamera : public AActor
{
	GENERATED_BODY()

public:
	/** SpringArm */
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	USpringArmComponent* SpringArmComponent;

	/** Camera */
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	UCameraComponent* CameraComponent;
	
	UPROPERTY()
	AActor* Player = nullptr;
	
	// Sets default values for this actor's properties
	ADOGPlayerCamera();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;
};
