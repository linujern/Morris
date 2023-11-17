// Fill out your copyright notice in the Description page of Project Settings.


#include "DOGEnemyLaserBot.h"

#include "Engine/SkeletalMeshSocket.h"
#include "Morris/Player/DOGPlayer.h"


// Sets default values
ADOGEnemyLaserBot::ADOGEnemyLaserBot()
{
	BeamCapsuleComponent = CreateDefaultSubobject<UCapsuleComponent>("Beam Capsule Collider");
	BeamCapsuleComponent->SetGenerateOverlapEvents(false);
	BeamCapsuleComponent->SetCollisionProfileName(TEXT("OverlapAllDynamic"));
	BeamCapsuleComponent->SetCanEverAffectNavigation(false);
	
	BeamCapsuleComponent->OnComponentBeginOverlap.AddDynamic(this, &ADOGEnemyLaserBot::OnOverlapBegin);
	BeamCapsuleComponent->OnComponentEndOverlap.AddDynamic(this, &ADOGEnemyLaserBot::OnOverlapEnd);
	
	BeamCapsuleComponent->SetupAttachment(MeshComponent, TEXT("QuickRigCharacter_RightHand"));
}

void ADOGEnemyLaserBot::OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
		//UKismetSystemLibrary::PrintString(GetWorld(), "OverlapBegin");
	if (OtherActor->IsA(ADOGPlayer::StaticClass()))
	{
		//UKismetSystemLibrary::PrintString(GetWorld(), "Direct Damage and start dot");
		
		// Deal Direct Damage
		Cast<ADOGPlayer>(GetTarget())->ApplyDamage(DirectDamage);
		
		// Start Dot Damage
		if (!DotTimerHandle.IsValid())
		{
			GetWorld()->GetTimerManager().SetTimer(DotTimerHandle, this, &ADOGEnemyLaserBot::DealDotDamage, DotTimer, true);
			bPlayerInBeam = true;
		}
	}
}

void ADOGEnemyLaserBot::OnOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
		//UKismetSystemLibrary::PrintString(GetWorld(), "OverlapEnd");
	if (OtherActor->IsA(ADOGPlayer::StaticClass()))
	{
		//UKismetSystemLibrary::PrintString(GetWorld(), "End Dot");

		// Remove damage
		GetWorld()->GetTimerManager().ClearTimer(DotTimerHandle);
		DotTimerHandle.Invalidate();
		
	}
}

void ADOGEnemyLaserBot::DealDotDamage()
{
	Cast<ADOGPlayer>(GetTarget())->ApplyDamage(DotDamage);
}

void ADOGEnemyLaserBot::StartBeam()
{
	//UKismetSystemLibrary::PrintString(GetWorld(), "StartBeam");
	BeamCapsuleComponent->SetGenerateOverlapEvents(true);
	//UKismetSystemLibrary::DrawDebugLine()
}

void ADOGEnemyLaserBot::StopBeam()
{
	//UKismetSystemLibrary::PrintString(GetWorld(), "EndBeam");
	BeamCapsuleComponent->SetGenerateOverlapEvents(false);

	if (bPlayerInBeam)
	{
		GetWorld()->GetTimerManager().ClearTimer(DotTimerHandle);
		DotTimerHandle.Invalidate();
	}
	
	bPlayerInBeam = false;
}

// Called when the game starts or when spawned
void ADOGEnemyLaserBot::BeginPlay()
{
	Super::BeginPlay();
	
}

