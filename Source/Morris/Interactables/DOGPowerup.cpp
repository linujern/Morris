// Fill out your copyright notice in the Description page of Project Settings.


#include "DOGPowerup.h"

#include "DOGWeapon.h"
#include "NiagaraComponentPool.h"
#include "Kismet/GameplayStatics.h"
#include "Morris/Player/DOGPlayer.h"
#include "Particles/ParticleSystem.h"
#include "Particles/ParticleSystemComponent.h"
#include "NiagaraFunctionLibrary.h"
#include "Kismet/KismetMaterialLibrary.h"


// Sets default values
ADOGPowerup::ADOGPowerup()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	//TODO: MAKE IT WORK lmao
}



// Called when the game starts or when spawned
void ADOGPowerup::BeginPlay()
{
	Super::BeginPlay();
	MeshComponent->SetSkeletalMesh(PowerupData->PowerupMesh);

	if (PowerupData->MaterialParameterCollection)
		UKismetMaterialLibrary::SetVectorParameterValue(GetWorld(), PowerupData->MaterialParameterCollection, "Color", PowerupData->PowerUpColor);
	
}

void ADOGPowerup::Interact(AActor* Caller)
{
	Super::Interact(Caller);

	if (bOnDuration || bOnCooldown)
		return;
			
	if (!Player)
		Player = Cast<ADOGPlayer>(Caller);
	
	switch (PowerupData->PowerUpType)
	{
	default:
		UE_LOG(LogTemp, Error, TEXT("Power Type defaulted - DOGPowerups Interact"))
		break;

	case HEALTH:
		{

			if (Player->ScrapMetal < PowerupData->BurstScrapCost)
				return;
			
			Player->ScrapMetal -= PowerupData->BurstScrapCost;
			Player->SaveScrapMetal();
			
			OnPowerBurst();
			PowerBurst();
			Player->ApplyDamage(-PowerupData->BurstEffect);
			GetWorld()->GetTimerManager().SetTimer(PowerHandle,this, &ADOGPowerup::HealPower, PowerupData->PowerTimerRate, true);
			StartDuration();

			bOnDuration = true;

			//UKismetSystemLibrary::PrintString(GetWorld(), "Heal");
			
		}
		break;

	case TIME:
		{
			if (Player->ScrapMetal < PowerupData->BurstScrapCost)
				return;
			Player->ScrapMetal -= PowerupData->BurstScrapCost;
			Player->SaveScrapMetal();
			
			OnPowerBurst();
			PowerBurst();
			// Change time flow
			Player->MovementComponent->MaxSpeed = Player->MovementComponent->GetMaxSpeed() * 2;
			
			if (ADOGWeapon* Weapon = Cast<ADOGWeapon>(Player->SecondaryEquipment))
				Weapon->bSpeedBoost = true;
			UGameplayStatics::SetGlobalTimeDilation(GetWorld(), PowerupData->BurstEffect);
			GetWorld()->GetTimerManager().SetTimer(PowerHandle, this, &ADOGPowerup::EndTimePower, PowerupData->PowerDuration * PowerupData->BurstEffect);
			GetWorld()->GetTimerManager().SetTimer(EffectHandle, this, &ADOGPowerup::PowerTimer, 2.5f * PowerupData->BurstEffect);
			
			StartDuration();
			bOnDuration = true;
			//UKismetSystemLibrary::PrintString(GetWorld(), "Time slow");
		}
		break;
	}
}

void ADOGPowerup::HealPower()
{
	//UKismetSystemLibrary::PrintString(GetWorld(), "Heal timer 1");
	// Cost scrap heal timer effect
	if (Player->ScrapMetal >= PowerupData->TimerScrapCost)
	{
		Player->ScrapMetal -= PowerupData->TimerScrapCost;
		Player->SaveScrapMetal();
		OnPowerTimer();
		PowerTimer();
		Player->ApplyDamage(-PowerupData->TimerEffect);
		//UKismetSystemLibrary::PrintString(GetWorld(), "Heal timer 2 - paid scrap");
		
	}
}

void ADOGPowerup::EndTimePower()
{
	//UKismetSystemLibrary::PrintString(GetWorld(), "Time reset");
	UGameplayStatics::SetGlobalTimeDilation(GetWorld(), 1);
	
	Player->MovementComponent->MaxSpeed = Player->MovementComponent->GetMaxSpeed() / 2;
	if (ADOGWeapon* Weapon = Cast<ADOGWeapon>(Player->SecondaryEquipment))
		Weapon->bSpeedBoost = false;
	if (ADOGWeapon* Weapon = Cast<ADOGWeapon>(Player->PrimaryEquipment))
		Weapon->bSpeedBoost = false;
	OnPowerTimer();
}

void ADOGPowerup::PowerBurst()
{
	if (PowerupData->ParticleSystem)
		UNiagaraFunctionLibrary::SpawnSystemAttached(PowerupData->ParticleSystem, Player->GetRootComponent(), NAME_None, FVector(0,0,-90), FRotator(0,0,0), EAttachLocation::KeepRelativeOffset, true, true);

	if (PowerupData->UseSound)
	UGameplayStatics::PlaySoundAtLocation(GetWorld(), PowerupData->UseSound, GetActorLocation());
}

void ADOGPowerup::PowerTimer()
{
	if (PowerupData->ParticleSystem)
		UNiagaraFunctionLibrary::SpawnSystemAttached(PowerupData->ParticleSystem, Player->GetRootComponent(), NAME_None, FVector(0,0,-90), FRotator(0,0,0), EAttachLocation::KeepRelativeOffset, true, true);

	if (PowerupData->TimerSound)
		UGameplayStatics::PlaySoundAtLocation(GetWorld(), PowerupData->TimerSound, GetActorLocation());
}

void ADOGPowerup::StartDuration()
{
	if (bOnDuration && bOnCooldown)
		return;

	OnDurationStart();
	GetWorld()->GetTimerManager().SetTimer(DurationHandle, this, &ADOGPowerup::EndDuration, PowerupData->PowerDuration + 0.2f, false);
	
	bOnDuration = true;

	//UKismetSystemLibrary::PrintString(GetWorld(), "Start Duration");
}

void ADOGPowerup::EndDuration()
{
	GetWorld()->GetTimerManager().ClearTimer(PowerHandle);
	PowerHandle.Invalidate();
	
	OnDurationEnd();
	
	StartCooldown();
	
	bOnDuration = false;
	if (PowerupData->StopUseSound)
		UGameplayStatics::PlaySoundAtLocation(GetWorld(), PowerupData->StopUseSound, GetActorLocation());

	//UKismetSystemLibrary::PrintString(GetWorld(), "End Duration");
}

void ADOGPowerup::StartCooldown()
{
	if (bOnCooldown)
		return;

	OnStartCooldown();
	GetWorld()->GetTimerManager().SetTimer(CooldownHandle, this, &ADOGPowerup::EndCooldown, (PowerupData->PowerCooldown), false);

	bOnCooldown = true;
	//UKismetSystemLibrary::PrintString(GetWorld(), "Start Cooldown");
}

void ADOGPowerup::EndCooldown()
{
	OnEndCooldown();
	bOnCooldown = false;
	CooldownHandle.Invalidate();
	//UKismetSystemLibrary::PrintString(GetWorld(), "End Cooldown");
}

void ADOGPowerup::CancelCooldown()
{
	bOnCooldown = false;
	GetWorld()->GetTimerManager().ClearTimer(CooldownHandle);
	CooldownHandle.Invalidate();
}

void ADOGPowerup::OnDurationStart_Implementation()
{
}

void ADOGPowerup::OnDurationEnd_Implementation()
{
}

void ADOGPowerup::OnStartCooldown_Implementation()
{
}

void ADOGPowerup::OnEndCooldown_Implementation()
{
}
