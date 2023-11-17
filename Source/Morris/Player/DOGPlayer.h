// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ArrowComponent.h"
#include "GameFramework/Pawn.h"
#include "Morris/Interactables/DOGInteractable.h"
#include "Morris/Interactables/DOGWorldInteractable.h"
#include "Morris/UI/DOGHud.h"
#include "MovementComponent/DOGMovementComponent.h"
#include "DOGPlayer.generated.h"

UCLASS()
class MORRIS_API ADOGPlayer : public APawn
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	ADOGPlayer();
	
	/** Capsule collider */
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	UCapsuleComponent* CapsuleComponent;
	
	/** Skeletal mesh */
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	USkeletalMeshComponent* MeshComponent;

	/** Movement component */
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	UDOGMovementComponent* MovementComponent;
	
	/** Arrow component */
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	UArrowComponent* ArrowComponent;

	/** Arrow component */
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	UArrowComponent* HandComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	float Health = 100.f;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	float MaxHealth = 100.f;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	float ScrapMetal = 0.f;

	UPROPERTY()
	float DamageMultiplier = 1.f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	float InteractionRadius = 600.f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	float PickupScrapMetalRadius = 100.f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	float DropDistance = 70.f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	float DropForce = 50.f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	UClass* WeaponPickupClass = nullptr;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	UClass* PowerPickupClass = nullptr;
	
	UPROPERTY()
	ADOGWorldInteractable* CurrentInteractableWorld = nullptr;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	ADOGInteractable* PrimaryEquipment = nullptr;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	ADOGInteractable* SecondaryEquipment = nullptr;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	float GamepadTurnRate = 12.f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	TSubclassOf<UDOGHud> HudWidgetClass = nullptr;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	USkeletalMesh* DeathMesh = nullptr;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	UDOGWeaponData* StartWeapon = nullptr;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	UDOGHud* HudWidget = nullptr;

	UPROPERTY()
	FVector TurnAtDirection;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	FRotator PlayerRotator = FRotator::ZeroRotator;
	
	UPROPERTY()
	bool bIsInteractingLocal = false;

	UPROPERTY()
	bool bIsDead = false;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	bool bIsUsingGamepad = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bIsInGameMouse = true;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	float DashLength = 200.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float MaxDash = 2.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float CurrentDash = 2.f;

	UPROPERTY()
	FTimerHandle DashTimerHandle;

	UPROPERTY()
	FTimerHandle ResetGameHandle;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float DashTimer = 5.f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	float FadeOutResetTime = 2.f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	float GameOverTimeDialation = 0.2f;

	UPROPERTY(BlueprintReadWrite)
	int RoomsCleared = 0;
	
	UPROPERTY()
	ADOGWorldInteractable* Item;
	
	UFUNCTION()
	void DashReplenish();
	
	UFUNCTION()
	void CheckForWorldInteractable();

	UFUNCTION()
	void TryInteractWorld();

	UFUNCTION()
	void TryInteractLocal();

	UFUNCTION()
	void StartInteractLocal();

	UFUNCTION()
	void EndInteractLocal();

	UFUNCTION()
	void TryReload();

	UFUNCTION()
	void TryPickupScrap();

	UFUNCTION()
	void ApplyDamage(float Value);

	UFUNCTION(BlueprintCallable)
	void DealDamageToPlayer(float Value);

	UFUNCTION()
	void SwapHands();

	UFUNCTION()
	void TryDropHand();

	UFUNCTION()
	void Dash();

	UFUNCTION()
	void ResetGame();

	UFUNCTION()
	void SaveScrapMetal();

	UFUNCTION(BlueprintImplementableEvent)
	void OnDash(FVector From, FVector To);

	UFUNCTION(BlueprintImplementableEvent)
	void OnDeath();

	UFUNCTION(BlueprintImplementableEvent)
	void OnFireGun(FVector Start, FVector End, bool Hit, const FHitResult& HitResult);

	UFUNCTION(BlueprintImplementableEvent)
	void OnDamage();
	
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	void AnyKey(FKey Key);
	
	void TurnRightLeftGamepad(float Rate);

	void TurnUpDownGamepad(float Rate);
	
	void LookUpAtRate(float Rate);
	
	void MoveForward(float Value);

	void MoveRight(float Value);
	
};
