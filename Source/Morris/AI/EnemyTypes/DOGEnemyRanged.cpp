// Fill out your copyright notice in the Description page of Project Settings.


#include "DOGEnemyRanged.h"

#include "AIController.h"
#include "DOGEnemyRangedBullet.h"
#include "Kismet/GameplayStatics.h"
#include "Morris/MorrisGameModeBase.h"


// Sets default values
ADOGEnemyRanged::ADOGEnemyRanged()
{
	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
}

// Called when the game starts or when spawned
void ADOGEnemyRanged::BeginPlay()
{
	Super::BeginPlay();
	//AIController->SetFocus(AIPawn->GetTarget(), EAIFocusPriority::Gameplay);
	
}

void ADOGEnemyRanged::FireBullet(UClass* Class)
{
	UDOGObjectPool* Pool = Cast<AMorrisGameModeBase>(UGameplayStatics::GetGameMode(GetWorld()))->PoolManager->GetPool("BulletPool");
	if (IsValid(Pool))
	{
		if (AActor* Bullet = Pool->TryGetFromPool())
        {
        	// Set Transform
        	Bullet->SetActorTransform(GetActorTransform());
        	Cast<ADOGEnemyRangedBullet>(Bullet)->FireInDirection(GetActorForwardVector());
        	OnFire(Bullet);
        }
        else
        {
        	const FTransform SpawnLocAndRotation = GetActorTransform();
        	Bullet = GetWorld()->SpawnActor<ADOGEnemyRangedBullet>(Class, SpawnLocAndRotation);
			OnFire(Bullet);
        }
		return;
	}
	
	const FTransform SpawnLocAndRotation = GetActorTransform();
    ADOGEnemyRangedBullet* Bullet = GetWorld()->SpawnActor<ADOGEnemyRangedBullet>(Class, SpawnLocAndRotation);
    
    OnFire(Bullet);
}

// Called every frame
void ADOGEnemyRanged::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

// Called to bind functionality to input
void ADOGEnemyRanged::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	
}

