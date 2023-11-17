// Fill out your copyright notice in the Description page of Project Settings.


#include "DOGRoomManager.h"

#include "DOGDoor.h"
#include "DOGRoom.h"
#include "NavigationSystem.h"
#include "AI/NavigationSystemBase.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Morris/AI/EnemyTypes/DOGEnemyRanged.h"
#include "Morris/Interactables/DOGChest.h"
#include "Morris/Player/DOGPlayer.h"
#include "Morris/UI/DOGSign.h"

void ADOGRoomManager::BeginPlay()
{
	Super::BeginPlay();
}

void ADOGRoomManager::Update()
{
	if(AllEnemiesDead())
	{
		//UKismetSystemLibrary::PrintString(GetWorld(), "New Wave");
		NewWave();
	}
}

bool ADOGRoomManager::AllEnemiesDead()
{
	bool bOut = true;
	for (const ADOGEnemyBase* Enemy: Enemies)
	{
		if (IsValid(Enemy))
		{
			//UKismetSystemLibrary::PrintString(GetWorld(), "Enemy alive");
			bOut = false;
		}
	}
	return bOut;
}

void ADOGRoomManager::NewWave()
{
	//TODO: Clean
	
	OnNewWave();
	
	//UKismetSystemLibrary::PrintString(GetWorld(), "Before pool check");
	bool bAllPoolsEmpty = Enemy1Amount + Enemy2Amount + Enemy3Amount <= 0;
	if (bAllPoolsEmpty)
	{
		OnClearEnemies();

		TArray<AActor*> Array;

		if (IsValid(RoomCurrent))
		{
			RoomCurrent->GetAttachedActors(Array);
			
			for (int i = 0; i < Array.Num(); i++)
			{
				 if (Array[i]->IsA(ADOGChest::StaticClass()))
				{
					ADOGChest* Chest = Cast<ADOGChest>(Array[i]);
					Chest->EnableChest();
					//UKismetSystemLibrary::PrintString(GetWorld(), "Chest thing 2");
				}
			}
		}
	}
	else 
	{
		//pull from available pool
		for (int i = EnemiesPerWave; i > 0; i--)
		{
			if (bAllPoolsEmpty)
			{
				break;
			}
			
			//rand choose a type until valid
			while (true)
			{
				const int Type = FMath::RandRange(1,3);
				if (Type == 1 && Enemy1Amount > 0)
				{
					Enemy1Amount--;
					SpawnEnemy(Enemy1Class);
					break;
				}
				if (Type == 2 && Enemy2Amount > 0)
				{
					Enemy2Amount--;
					SpawnEnemy(Enemy2Class);
					break;
				}
				if (Type == 3 && Enemy3Amount > 0)
				{
					Enemy3Amount--;
					SpawnEnemy(Enemy3Class);
					break;
				}
				
				//anti hardlock
				break;
			}
		}
	}
}

void ADOGRoomManager::SpawnEnemy(TSubclassOf<ADOGEnemyBase> Type)
{
	//UKismetSystemLibrary::PrintString(GetWorld(), "Spawn called");
	
	FNavLocation OutNavLocation;
	UNavigationSystemV1* NavSys = FNavigationSystem::GetCurrent<UNavigationSystemV1>(GetWorld());
	if (NavSys)
	{
		//UKismetSystemLibrary::PrintString(GetWorld(), "Spawn");
		if(NavSys->GetRandomReachablePointInRadius(this->GetActorLocation(), SpawnRadius, OutNavLocation))
		{
			FTransform Transform;
			Transform.SetLocation(OutNavLocation.Location + FVector(0,0,EnemySpawnOffsetZ));
			Enemies.Add(GetWorld()->SpawnActor<ADOGEnemyBase>(Type, Transform));
		}
	}
}



void ADOGRoomManager::RoomCleared()
{
	bShouldCallUpdate = false;

	//Spawn chest
	UKismetSystemLibrary::PrintString(GetWorld(), "RoomCleared called");
	
	//Open doors except the entered and/or bottom
	if (DoorUp && !DoorUp->bIsEntrance)
	{
		DoorUp->bIsOpen = true;
		DoorUp->OnDoorOpen();
	}
	if (DoorLeft && !DoorLeft->bIsEntrance)
	{
		DoorLeft->bIsOpen = true;
		DoorLeft->OnDoorOpen();
	}
	if (DoorRight && !DoorRight->bIsEntrance)
	{
		DoorRight->bIsOpen = true;
		DoorRight->OnDoorOpen();
	}

	
	ADOGPlayer* Player= Cast<ADOGPlayer>(UGameplayStatics::GetActorOfClass(GetWorld(), ADOGPlayer::StaticClass()));
	Player->RoomsCleared++;
	
}

void ADOGRoomManager::ToogleAllDoors(bool Open)
{
	DoorUp->bIsOpen = Open;
	DoorLeft->bIsOpen = Open;
	DoorRight->bIsOpen = Open;
	DoorBottom->bIsOpen = Open;
}

void ADOGRoomManager::EnteredRoom()
{
	OnEnterRoom();

	bShouldCallUpdate = true;
	
	
	//Close all doors
	ToogleAllDoors(false);
	DoorUp->OnDoorClose();
	DoorLeft->OnDoorClose();
	DoorRight->OnDoorClose();
	DoorBottom->OnDoorClose();
	
	if (IsValid(RoomCurrent->UpWidget->GetClass()) && IsValid(RoomUp))
		RoomCurrent->UpWidget->SetMaterial(0, RoomUp->IconMaterial);
	if (IsValid(RoomCurrent->RightWidget->GetClass()) && IsValid(RoomRight))
		RoomCurrent->RightWidget->SetMaterial(0, RoomRight->IconMaterial);
	if (IsValid(RoomCurrent->LeftWidget->GetClass()) && IsValid(RoomLeft))
		RoomCurrent->LeftWidget->SetMaterial(0, RoomLeft->IconMaterial);
	
	NewWave();
}

void ADOGRoomManager::ExitedRoom()
{
	bShouldCallUpdate = false;

	//Close all doors
	ToogleAllDoors(false);
	DoorUp->OnDoorClose();
	DoorLeft->OnDoorClose();
	DoorRight->OnDoorClose();
	DoorBottom->OnDoorClose();

	//RoomCurrent->RoomSize->SetCollisionProfileName(TEXT("BlockAllDynamic"));
	
	OnExitRoom();
}

void ADOGRoomManager::SetupDoors(ADOGDoor* Top, ADOGDoor* Right, ADOGDoor* Left, ADOGDoor* Bottom)
{
	DoorUp = Top;
	DoorRight = Right;
	DoorLeft = Left;
	DoorBottom = Bottom;
}

void ADOGRoomManager::SpawnRoomSigns()
{
	RoomUp->RoomType;
	RoomLeft->RoomType;
	RoomRight->RoomType;
}

void ADOGRoomManager::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	if (bShouldCallUpdate || bIsBossRoom)
	{
		//can slow this with timer
		Update();

		
		//UKismetSystemLibrary::PrintString(GetWorld(), "Update");
	}
}
