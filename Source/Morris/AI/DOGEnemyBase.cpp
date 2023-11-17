// Fill out your copyright notice in the Description page of Project Settings.


#include "DOGEnemyBase.h"

#include <string>

#include "AIController.h"
#include "DOGAIController.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Morris/Player/DOGPlayer.h"


// Sets default values
ADOGEnemyBase::ADOGEnemyBase()
{
	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
	
	CapsuleComponent = CreateDefaultSubobject<UCapsuleComponent>("Capsule Collider");
	CapsuleComponent->SetCapsuleHalfHeight(88.f);
	CapsuleComponent->SetCapsuleRadius(44.f);
	CapsuleComponent->SetGenerateOverlapEvents(true);
	CapsuleComponent->SetCollisionProfileName(TEXT("BlockAllDynamic"));
	CapsuleComponent->SetCanEverAffectNavigation(false);
	RootComponent = CapsuleComponent;

	MeshComponent = CreateDefaultSubobject<USkeletalMeshComponent>("Mesh");
	MeshComponent->SetupAttachment(CapsuleComponent);
	
	MovementComponent = CreateDefaultSubobject<UDOGMovementComponent>("DOG Movement");
	//MovementComponent.
	//MovementComponent->RegisterComponent();
	//if(MovementComponent)
    //		MovementComponent->SetUpdatedComponent(RootComponent);
	
}

void ADOGEnemyBase::ApplyDamage(float Damage)
{

	if (bInvulnerable)
		return;
	
	//TODO: Implement
	HP -= Damage;
	OnEnemyTakeDamagee(HPMax, HP);
	
	//UKismetSystemLibrary::PrintString(GetWorld(), "Hit Enemy - Current Health: " + FString::FromInt(HP));
	
	if (HP <= 0)
	{
		// Die
		OnEnemyDeath();
		// ADOGRM RM = UGameplayStatics::GetActorOfClass(GetWorld(), ADOGRM::StaticClass()); // GET ROOM MANAGER FOR DEATH COUNT
		// RM.OnEnemyDeath(); // Trigger RM's thingy

		//Money
		for (int i = 0; i < FMath::RandRange(MinMoneyDrop,MaxMoneyDrop); ++i)
		{
			if(ScrapMetalClass)
			{
				const FTransform SpawnLocAndRotation = GetActorTransform();
				ADOGScrapMetal* Money = GetWorld()->SpawnActorDeferred<ADOGScrapMetal>(ScrapMetalClass, SpawnLocAndRotation);
				Money->Value = FMath::FRandRange(MinMoneyValue, MaxMoneyValue);

				if (bIsGolden)
				{
					Money->Value *= 2.f;
					Money->bIsGolden = true;
				}
				
				Money->FinishSpawning(SpawnLocAndRotation);

				FVector Impulse = UKismetMathLibrary::RandomUnitVector();
				
				Money->MeshComponent->AddImpulse(Impulse * FMath::RandRange(MinMoneyImpulse, MaxMoneyImpulse), NAME_None, true);
			}
		}
		
		Destroy();
	}
}

void ADOGEnemyBase::StopInvulnerable()
{
	bInvulnerable = false;
	InvulnerableHandle.Invalidate();
}

// Called when the game starts or when spawned
void ADOGEnemyBase::BeginPlay()
{
	Super::BeginPlay();

	HP = HPMax;
	
	Target = UGameplayStatics::GetActorOfClass(GetWorld(), ADOGPlayer::StaticClass());

	bInvulnerable = true;
	GetWorld()->GetTimerManager().SetTimer(InvulnerableHandle, this, &ADOGEnemyBase::StopInvulnerable, 2.5f);
	
	OnEnemySpawn();
}



// Called every frame
void ADOGEnemyBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
}

