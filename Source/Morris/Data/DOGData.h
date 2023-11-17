#pragma once

#include "DOGData.generated.h"

UENUM()
enum EEnemyType
{
	RANGED,
	MELEE,
	BOMB,
};

UENUM()
enum EWeaponType
{
	PISTOL,
	SHOTGUN,
	SMG,
	AR,
	SNIPER,
};

UENUM()
enum EShopItemType
{
	WEAPON,
	POWERUP,
	STAT // So it isnt Weapon or Powerup in the data
	//TODO: Ask linus
};

UENUM()
enum EInputType
{
	PC,
	XBOX,
	PLAYSTATION
};

UENUM()
enum ERoomType
{
	RTMerchant,
	RTGun,
	RTUpgrade,
	RTScrap,
	RTBoss,
	RTEmpty,
};

UENUM()
enum EPowerUpType
{
	HEALTH,
	TIME
	
};

UENUM()
enum EStatUpgrades
{
	HEALTHUPGRADE,
	DAMAGEUPGRADE,
	//SPEEDUPGRADE,
	DASHUPGRADE
};

UENUM()
enum EShopType
{
	ALLSHOP,
	WEAPONSHOP,
	POWERSHOP,
	STATSHOP,
	WEAPONPOWERSHOP,
	WEAPONSTATSHOP,
	POWERSTATSHOP
};
