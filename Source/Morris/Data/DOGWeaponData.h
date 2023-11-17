// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "DOGData.h"
#include "DOGItemData.h"
#include "Curves/CurveFloat.h"
#include "Kismet/GameplayStatics.h"
#include "Math/Range.h"

#include "DOGWeaponData.generated.h"

class USoundCue;
/**
 * 
 */
UCLASS()
class MORRIS_API UDOGWeaponData : public UDOGItemData
{
	GENERATED_BODY()

public:

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Weapon",
		meta=(ToolTip="The type of this weapon. Only used for data."))
	TEnumAsByte<EWeaponType> WeaponType;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Weapon",
		meta=(ToolTip="Base damage dealt to enemies. Used for calculation."))
	float BaseDamage = 10.f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Weapon",
		meta=(ToolTip="Base ammo for weapon.\n\nNote: This also acts as max ammo, after reload gun will be set back to BaseAmmo."))
	float BaseAmmo = 30.f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Weapon",
		meta=(ToolTip="The rate of which a weapon can be fired.\nActs as a cooldown, example:\nA fire rate of 0.2 will give the gun a 0.2 second cooldown in-between shots."))
	float FireRate = 0.2f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Weapon",
		meta=(ToolTip="Base unit range for weapon.\n\nNote: This uses Unreal unit measurements, 1 unit = 1 cm in game space."))
	float Range = 600.f;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Weapon",
		meta=(ToolTip="Unit kickback for player after shooting weapon.\n\nNote: This uses Unreal unit measurements, 1 unit = 1 cm in game space."))
	float PlayerKickback = 100.f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Weapon",
		meta=(ToolTip="The time (in seconds) it takes for a weapon to reload."))
	float ReloadSpeed = 4.f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Weapon", DisplayName = "Accuracy Curve",
		meta=(ToolTip="Accuracy curve.\n\nTimeline from 0.f to 1.f.\n\nWeapon will get a random value from 0-1 on the timeline when evaluating accuracy.\nThe value retrived closer to 0 will be more accurate, closer to 1 will be totally inaccurate."))
	FRuntimeFloatCurve AccuracyCurve;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Weapon", DisplayName = "Damage Falloff Curve",
    		meta=(ToolTip="Damagr falloff.\n\nTimeline from 0.f to 1.f.\n\nBased on weapon range % from 0.f to 1.f on the timeline damage will be reduce.\n\n0.f = 100% damage\n1.f = 0% damage"))
    FRuntimeFloatCurve DamageFalloff;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Weapon",
		meta=(ToolTip="The mesh that this gun will use."))
	USkeletalMesh* GunMesh = nullptr;

	//TODO: Animations

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Weapon",
		meta=(ToolTip="The sound that this gun will play when shot."))
	USoundBase* ShootSound = nullptr;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Weapon",
		meta=(ToolTip="The sound that this gun will play when reload starts."))
	USoundBase* ReloadSound = nullptr;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Weapon",
		meta=(ToolTip="Offset where the gun fires from. Used to fine-tune where bullets travel from the gun."))
	float FiringOffsetForward;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Weapon",
		meta=(ToolTip="Offset where the gun fires from. Used to fine-tune where bullets travel from the gun."))
	float FiringOffsetUpward;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Weapon|Multishot",
		meta=(ToolTip="Enable multishot.\n\nNote: This will make the weapon shoot extra bullets per ammo."))
	bool bEnableMultishot = false;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Weapon|Multishot",
		meta=(ToolTip="Amount of shots per ammo."))
	float MultiShots = 3.f;
	
};
