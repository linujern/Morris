// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/Actor.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "DOGEnemyRangedBullet.generated.h"

UCLASS()
class MORRIS_API ADOGEnemyRangedBullet : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	ADOGEnemyRangedBullet();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	void FireInDirection(const FVector& ShootDirection);

	void TimerEnd();
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Projectile")
	float BulletDamage = 20.0f;
	
	UPROPERTY(VisibleAnywhere, Category="Movement")
	UProjectileMovementComponent* ProjectileMovementComponent;

	UFUNCTION(BlueprintCallable)
	void OverlapCheck(  AActor* OtherActor);

	
	UPROPERTY(VisibleAnywhere, Category="Projectile")
	UStaticMeshComponent* StaticMeshComponent;
	
private:


};
