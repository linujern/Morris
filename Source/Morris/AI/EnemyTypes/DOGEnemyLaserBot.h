// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "Morris/AI/DOGEnemyBase.h"
#include "DOGEnemyLaserBot.generated.h"

UCLASS()
class MORRIS_API ADOGEnemyLaserBot : public ADOGEnemyBase
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	ADOGEnemyLaserBot();


	
	UFUNCTION()
	void OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult);
	UFUNCTION()
	void OnOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	UFUNCTION()
	void DealDotDamage();

	UFUNCTION()
	void StartBeam();
	UFUNCTION()
	void StopBeam();
	
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:

	/** Beam collider */
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	UCapsuleComponent* BeamCapsuleComponent;

	UPROPERTY()
	FTimerHandle DotTimerHandle;
	
	UPROPERTY(VisibleAnywhere, Category="Enemy|Stats|LaserBot")
	bool bPlayerInBeam = false;

	// Damage on contact
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Enemy|Stats|LaserBot")
	float DirectDamage = 30.f;
	
	// Damage on stay in laser
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Enemy|Stats|LaserBot")
	float DotDamage = 10.f;
	
	// Damage on stay in laser
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Enemy|Stats|LaserBot")
	float DotTimer = 0.5f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bAttackAnimation = false;
};
