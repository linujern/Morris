// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Morris/Data/DOGWeaponData.h"
#include "Morris/UI/DOGItemUI.h"
#include "DOGWorldInteractable.generated.h"

UCLASS()
class MORRIS_API ADOGWorldInteractable : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	ADOGWorldInteractable();

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	USkeletalMeshComponent* MeshComponent;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	UStaticMeshComponent* StaticMeshComponent = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<UDOGItemUI> HudWidgetClass = nullptr;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	UDOGItemUI* HudWidget = nullptr;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	bool bCanInteract = true;
	
	UFUNCTION()
	void ToggleOutline(bool bIsVisible);

	UFUNCTION()
	void SetItemUI();
	
	UFUNCTION(BlueprintCallable)
	ADOGWorldInteractable* InRangeOfItem();

	UFUNCTION(BlueprintCallable)
	ADOGWorldInteractable* NotInRangeOfItem();
	
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	
	virtual void Interact(AActor* Caller);
	
};
