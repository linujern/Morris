// Fill out your copyright notice in the Description page of Project Settings.


#include "DOGEnemyRangedBullet.h"

//#include "Components/CapsuleComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Morris/MorrisGameModeBase.h"
#include "Morris/AI/DOGEnemyBase.h"
#include "Morris/Generation/DOGRoom.h"
#include "Morris/Player/DOGPlayer.h"


// Sets default values
ADOGEnemyRangedBullet::ADOGEnemyRangedBullet()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
	
	
	StaticMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>("MeshComponent");
	StaticMeshComponent->SetCollisionProfileName(TEXT("OverlapAll"));
	RootComponent = StaticMeshComponent;
	//StaticMeshComponent->SetStaticMesh()
	
	if (!ProjectileMovementComponent)
	{
		ProjectileMovementComponent = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileMovementComponent"));
		ProjectileMovementComponent->SetUpdatedComponent(StaticMeshComponent);
		
		ProjectileMovementComponent->bRotationFollowsVelocity = false;
		ProjectileMovementComponent->ProjectileGravityScale = 0.0f;
	}

	//StaticMeshComponent->OnComponentBeginOverlap.AddDynamic(this, &ADOGEnemyRangedBullet::OnBeginOverlap);

	
	//if (!MeshComponent)
	//{
	//	MeshComponent = CreateDefaultSubobject<USkeletalMeshComponent>("Mesh");
	//	MeshComponent->SetupAttachment(CollisionComponent);
	//
	//}

}

// Called when the game starts or when spawned
void ADOGEnemyRangedBullet::BeginPlay()
{
	Super::BeginPlay();
	FTimerHandle TimerHandle;
	GetWorldTimerManager().SetTimer(TimerHandle, this, &ADOGEnemyRangedBullet::TimerEnd, 5.f, false);
}

// Called every frame
void ADOGEnemyRangedBullet::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	//DrawDebugSphere(GetWorld(), this->GetTransform().GetLocation(), 10.0f, char16_t(), FColor::Black, false, 1, 0,0);
}

void ADOGEnemyRangedBullet::FireInDirection(const FVector& ShootDirection)
{
	ProjectileMovementComponent->Velocity = ShootDirection * ProjectileMovementComponent->InitialSpeed;
}

void ADOGEnemyRangedBullet::TimerEnd()
{
	UDOGObjectPool* Pool = Cast<AMorrisGameModeBase>(UGameplayStatics::GetGameMode(GetWorld()))->PoolManager->GetPool("BulletPool");
	if (Pool->ActorsInPool.Contains(this))
		return;
	ProjectileMovementComponent->StopMovementImmediately();
	Pool->AddToPool(this);
	
}

void ADOGEnemyRangedBullet::OverlapCheck( AActor* OtherActor)
{
// TODO: Change Cast and fix object pool

	
	if (OtherActor->IsA( ADOGPlayer::StaticClass()))
	{
		//UKismetSystemLibrary::PrintString(GetWorld(), "Player Damaged");
		ADOGPlayer* Player = Cast<ADOGPlayer>(OtherActor);
		Player->ApplyDamage(BulletDamage);
		
	}
//(OtherActor->IsA(ADOGEnemyBase::StaticClass()) || OtherActor->IsA(ADOGEnemyRanged::StaticClass() || OtherActor->IsA(ADOGRoom::StaticClass()) || OtherActor->IsA(ADOGInteractable::StaticClass() || OtherActor->IsA(ADOGWorldInteractable::StaticClass()))))

	else if (OtherActor->IsA(ADOGEnemyBase::StaticClass()))
	{
		return;
	}
	else if (OtherActor->IsA(StaticClass()))
	{
		return;
	}
	else if (OtherActor->IsA(ADOGRoom::StaticClass()))
	{
		return;
	}
	else if (OtherActor->IsA(ADOGInteractable::StaticClass()))
	{
		return;
	}
	else if (OtherActor->IsA(ADOGWorldInteractable::StaticClass()))
	{
		return;
	}
	
	
	//else if (Cast<ADOGEnemyBase>(OtherActor) || Cast<ADOGEnemyRangedBullet>(OtherActor) || Cast<ADOGRoom>(OtherActor) || Cast<ADOGInteractable>(OtherActor) || Cast<ADOGWorldInteractable>(OtherActor))
	//{
	//	//UKismetSystemLibrary::PrintString(GetWorld(), "OverlapReturned");
	//	return;
	//}
	
	ProjectileMovementComponent->StopMovementImmediately();
	Cast<AMorrisGameModeBase>(UGameplayStatics::GetGameMode(GetWorld()))->PoolManager->GetPool("BulletPool")->AddToPool(this);
	//Destroy();
}

