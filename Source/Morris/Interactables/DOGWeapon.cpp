// Fill out your copyright notice in the Description page of Project Settings.


#include "DOGWeapon.h"

#include "Kismet/KismetMathLibrary.h"
#include "Morris/AI/DOGEnemyBase.h"
#include "Morris/Player/DOGPlayer.h"


// Sets default values
ADOGWeapon::ADOGWeapon()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	//MeshComponent->SetupAttachment(this->GetRootComponent());
	//MeshComponent->RegisterComponent();
}

// Called when the game starts or when spawned
void ADOGWeapon::BeginPlay()
{
	Super::BeginPlay();
	MeshComponent->SetSkeletalMesh(WeaponData->GunMesh);
	CurrentAmmo = WeaponData->BaseAmmo;
	UE_LOG(LogTemp, Warning, TEXT("Ammo: %f"), WeaponData->BaseAmmo);
}

void ADOGWeapon::Interact(AActor* Caller)
{
	if(bOnFireCooldown || bIsReloading || !CanShoot(Caller)) return;

	//Firerate cooldown
	GetWorld()->GetTimerManager().SetTimer(FireCooldownHandle, this, &ADOGWeapon::ResetFirecooldown, WeaponData->FireRate, false);
	bOnFireCooldown = true;

	CurrentAmmo--;
	
	FVector Start = (GetActorLocation() + (Cast<ADOGPlayer>(Caller)->MeshComponent->GetRightVector() * WeaponData->FiringOffsetForward)) + FVector(0,0,WeaponData->FiringOffsetUpward);

	FVector End;
	
	FVector Accuracy;

	int Shots = 1;
	if(WeaponData->bEnableMultishot)
		Shots = WeaponData->MultiShots;

	TArray<ADOGEnemyBase*> HitTargets;
	
	for (int i = 0; i < Shots; ++i)
		{
		//Accuracy
		if (WeaponData->AccuracyCurve.ExternalCurve != nullptr)
		{
			Accuracy.X = WeaponData->AccuracyCurve.ExternalCurve->GetFloatValue(UKismetMathLibrary::RandomFloatInRange(0.f,1.f));
			Accuracy.Y = WeaponData->AccuracyCurve.ExternalCurve->GetFloatValue(UKismetMathLibrary::RandomFloatInRange(0.f,1.f));
			Accuracy.Z = WeaponData->AccuracyCurve.ExternalCurve->GetFloatValue(UKismetMathLibrary::RandomFloatInRange(0.f,1.f));
		}
		else
		{
			Accuracy.X = WeaponData->AccuracyCurve.GetRichCurveConst()->Eval(UKismetMathLibrary::RandomFloatInRange(0.f,1.f));
			Accuracy.Y = WeaponData->AccuracyCurve.GetRichCurveConst()->Eval(UKismetMathLibrary::RandomFloatInRange(0.f,1.f));
			Accuracy.Z = WeaponData->AccuracyCurve.GetRichCurveConst()->Eval(UKismetMathLibrary::RandomFloatInRange(0.f,1.f));
		}
		
		//Accuracy multiplier
		Accuracy *= 100.f;
		
		if(UKismetMathLibrary::RandomBool())
			Accuracy.X = -Accuracy.X;
		if(UKismetMathLibrary::RandomBool())
			Accuracy.Y = -Accuracy.Y;
		if(UKismetMathLibrary::RandomBool())
			Accuracy.Z = -Accuracy.Z;
		
		ADOGPlayer* Player = Cast<ADOGPlayer>(Caller);
		
		if(Target)
			End = Target->GetActorLocation() + Accuracy;
		else
			End = (Start + (Player->MeshComponent->GetRightVector() * WeaponData->Range)) + Accuracy;

		TArray<AActor*> Ignored;
		Ignored.Add(this);
		Ignored.Add(Caller);

		FHitResult Hit;
	
		if(UKismetSystemLibrary::LineTraceSingle(GetWorld(), Start, End, TraceTypeQuery1, true, Ignored, EDrawDebugTrace::None, Hit, true, FLinearColor::Red, FLinearColor::Green, 0.1))
		{
			if(ADOGEnemyBase* HitEnemy = Cast<ADOGEnemyBase>(Hit.GetActor()))
				HitTargets.Add(HitEnemy);
		}

		Player->OnFireGun(Start, End, Hit.bBlockingHit, Hit);
		
		if (CurrentAmmo <= 0)
			StartReload();
	}
	
	UGameplayStatics::PlaySoundAtLocation(GetWorld(), WeaponData->ShootSound, GetActorLocation());

	//TODO: Kickback

	if(HitTargets.Num() > 0)
	{
		UE_LOG(LogTemp, Warning, TEXT("Hit enemy"));
		for(ADOGEnemyBase* Enemy : HitTargets)
		{
			//Damage falloff
			float Falloff;
			float TimeAt = UKismetMathLibrary::Vector_Distance(Enemy->GetActorLocation(), Caller->GetActorLocation()) / WeaponData->Range;
			if (WeaponData->DamageFalloff.ExternalCurve != nullptr)
				Falloff = WeaponData->DamageFalloff.ExternalCurve->GetFloatValue(TimeAt);
			else
				Falloff = WeaponData->DamageFalloff.GetRichCurveConst()->Eval(TimeAt);

			//Damage
			Enemy->ApplyDamage((WeaponData->BaseDamage * Cast<ADOGPlayer>(Caller)->DamageMultiplier) * (1.f - Falloff));
		}
	}
}

void ADOGWeapon::AimAssist()
{
	//TODO: Aim assist
}

void ADOGWeapon::StartReload()
{
	if (bIsReloading)
		return;

	OnStartReload();

	if (bSpeedBoost)
		GetWorld()->GetTimerManager().SetTimer(ReloadTimerHandle, this, &ADOGWeapon::EndReload, WeaponData->ReloadSpeed / 2, false);
	else
		GetWorld()->GetTimerManager().SetTimer(ReloadTimerHandle, this, &ADOGWeapon::EndReload, WeaponData->ReloadSpeed, false);

	UGameplayStatics::PlaySoundAtLocation(GetWorld(), WeaponData->ReloadSound, GetActorLocation());
	
	bIsReloading = true;
}

void ADOGWeapon::EndReload()
{
	OnEndReload();
	CurrentAmmo = WeaponData->BaseAmmo;
	bIsReloading = false;
	ReloadTimerHandle.Invalidate();
}

void ADOGWeapon::ResetFirecooldown()
{
	bOnFireCooldown = false;
	FireCooldownHandle.Invalidate();
}

bool ADOGWeapon::CanShoot(AActor* Caller)
{
	if (CurrentAmmo > 0) return true;
	Cast<ADOGPlayer>(Caller)->TryReload();
	return false;
}

bool ADOGWeapon::TryCancelReload()
{
	if(ReloadTimerHandle.IsValid())
	{
		GetWorld()->GetTimerManager().ClearTimer(ReloadTimerHandle);
		ReloadTimerHandle.Invalidate();
		bIsReloading = false;
		return true;
	}
	return false;
}

void ADOGWeapon::OnEndReload_Implementation()
{
	//
}

void ADOGWeapon::OnStartReload_Implementation()
{
	//
}

// Called every frame
void ADOGWeapon::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

