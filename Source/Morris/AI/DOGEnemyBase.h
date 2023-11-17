// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/Pawn.h"
#include "Morris/Interactables/Money/DOGScrapMetal.h"
#include "Morris/Player/MovementComponent/DOGMovementComponent.h"
#include "DOGEnemyBase.generated.h"

UCLASS()
class MORRIS_API ADOGEnemyBase : public APawn
{
	GENERATED_BODY()

public:

	// Sets default values for this pawn's properties
	ADOGEnemyBase();

	UFUNCTION(BlueprintCallable)
	void ApplyDamage(float Damage);

	UFUNCTION(BlueprintCallable)
	AActor* GetTarget() { return Target;};

	UFUNCTION(BlueprintCallable)
	FVector GetPos() { return this->GetTransform().GetLocation(); }

	UFUNCTION()
	void StopInvulnerable();
	
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

private:

	UPROPERTY(VisibleAnywhere, Category="Enemy")
	float HP;
	
	UPROPERTY(VisibleAnywhere, Category="Enemy|Debug")
	FVector MovementDirection;
	
	UPROPERTY(EditAnywhere, Category="Enemy|Debug")
	AActor* Target;

	UPROPERTY()
	FTimerHandle InvulnerableHandle;
public:
	
	/** Capsule collider */
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	UCapsuleComponent* CapsuleComponent;
	
	/** Skeletal mesh */
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	USkeletalMeshComponent* MeshComponent;

	/** Movement component */
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	UDOGMovementComponent* MovementComponent;

	
	// Blueprint Implementable
	UFUNCTION(BlueprintImplementableEvent)
	void OnEnemyTakeDamagee(float HealthMax, float CurrentHealth);
	UFUNCTION(BlueprintImplementableEvent)
	void OnEnemyDeath();
	UFUNCTION(BlueprintImplementableEvent)
	void OnEnemySpawn();
	UFUNCTION(BlueprintImplementableEvent)
	void OnEnemyAttack();
	UFUNCTION(BlueprintImplementableEvent)
	void OnTriggerAnimation();

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Enemy")
	bool bInvulnerable;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(ExposeOnSpawn="true"))
	bool bIsGolden = false;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Enemy|Stats")
	float HPMax = 100.0f;
	
	// If player is in this range it triggers the attack - If 0 sets in code to default value 
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Enemy|Stats")
	float ActivateAttackRange = 0.0f;
	
	// Will determine the max distance of random movement the enemy makes - If 0 sets in code to example value
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Enemy|Stats")
	float RandomMoveRange = 0.0f;

	// If the player is in this range the enemy will finish its move to - If 0 sets in code to default value
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Enemy|Stats")
	float FinishMoveToRange = 0.0f;
	
	// If the player is in this range the enemy will move away - If 0 sets in code to default value
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Enemy|Stats")
	float BackAwayRange = 0.0f;

	// If player is in this range activate stuff - If 0 sets in code to default value - Some are not affected by this - Currently affects the chasing range of BOOMBOT
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Enemy|Stats")
	float InRange = 0.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Enemy|Stats")
	float StrafeRange = 400.0f;
	
	// Delays the attack for the animations
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Enemy|Stats")
	float AttackAnimationDelay = 0.0f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Enemy|Stats|Money")
	int MinMoneyDrop = 4;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Enemy|Stats|Money")
	int MaxMoneyDrop = 12;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Enemy|Stats|Money")
	float MinMoneyValue = 1.f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Enemy|Stats|Money")
	float MaxMoneyValue = 4.f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Enemy|Stats|Money")
	float MinMoneyImpulse = 8.f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Enemy|Stats|Money")
	float MaxMoneyImpulse = 16.f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Enemy|Stats|Money")
	TSubclassOf<ADOGScrapMetal> ScrapMetalClass = nullptr;
	
	
};
