// Fill out your copyright notice in the Description page of Project Settings.


#include "DOGGenerator.h"

#include "NavigationSystem.h"
#include "Engine/AssetManager.h"
#include "Kismet/GameplayStatics.h"
#include "Morris/DOGGameInstance.h"
#include "Morris/Data/DOGRoomData.h"
#include "Morris/Player/DOGPlayer.h"
#include "NavMesh/NavMeshBoundsVolume.h"


// Sets default values
ADOGGenerator::ADOGGenerator()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

void ADOGGenerator::OnRoomLoaded(FPrimaryAssetId RoomAssetId)
{
	UAssetManager& AssetManager = UAssetManager::Get();
	UObject* AssetObject = AssetManager.GetPrimaryAssetObject(RoomAssetId);
	UDOGRoomData* Room = Cast<UDOGRoomData>(AssetObject);
	
	AddToRoomCorrectArray(Room);
	AssetAmount--;
}

void ADOGGenerator::GenerateLevelLayout(int32 LevelLength)
{
	int LevelRowLength = 2 * LevelLength /*+ 1*/; 
	int LevelColumnLength = LevelLength /*+ 1*/;

	LevelLayout.Empty();

	for (int y = 0; y <= LevelColumnLength; y++)
	{
		for (int x = 0; x <= LevelRowLength; x++)
		{
			UDOGRoomData* NewRoom = GetValidRoom(x, y, LevelLength);

			if (x == LevelLength && y == LevelLength)
			{
				//TODO START LEVEL
				NewRoom = MerchantRooms[MerchantRooms.Num() - 1];
			}
			
			LevelLayout.Add(NewRoom);
			
			ADOGRoom* RoomLeftIfValid = nullptr;
			ADOGRoom* RoomUpIfValid = nullptr;

			//Check RoomLeft for connection
			if (x - 1 >= 0)
			{
				RoomLeftIfValid = LevelLayoutBuilt[RoomPos(x - 1, y, LevelLength)];
			}
			
			//Check RoomUp for connection
			if (y - 1 >= 0)
			{
				RoomUpIfValid = LevelLayoutBuilt[RoomPos(x, y - 1, LevelLength)];
			}
			
			//spawn Room
			FVector Translate(x * RoomSpacingX, y * RoomSpacingY, 0);
			FRotator Rotate(0,270,0);
			FTransform RoomTransform;
			RoomTransform.SetLocation(Translate);

			ADOGRoom* SpawnedRoom = Cast<ADOGRoom>(GetWorld()->SpawnActorDeferred<ADOGRoom>(NewRoom->RoomObject, RoomTransform));

			SpawnedRoom->RoomLeft = RoomLeftIfValid;
			SpawnedRoom->RoomUp = RoomUpIfValid;
			SpawnedRoom->RoomType = NewRoom->RoomType;

			SpawnedRoom->SetActorHiddenInGame(true);
			SpawnedRoom->SetAllDoorsHidden(true);
			SpawnedRoom->SetActorTickEnabled(false);

			SpawnedRoom->FinishSpawning(RoomTransform);

			SpawnedRoom->SetActorRotation(Rotate);
			
			SpawnedRoom->AttachToActor(this, FAttachmentTransformRules::KeepRelativeTransform);
			
			LevelLayoutBuilt.Add(SpawnedRoom);

			if (IsValid(RoomLeftIfValid))
			{
				RoomLeftIfValid->RoomManager->RoomRight = SpawnedRoom;
				RoomLeftIfValid->RoomRight = SpawnedRoom;
			}
			if (IsValid(RoomUpIfValid))
			{
				RoomUpIfValid->RoomManager->RoomDown = SpawnedRoom;
				RoomUpIfValid->RoomDown = SpawnedRoom;
			}
		}
	}

	FTransform Transform;
	ADOGRoom* SpawnedRoom = Cast<ADOGRoom>(GetWorld()->SpawnActorDeferred<ADOGRoom>(StartingRoomClass, Transform));

	//Check RoomLeft for connection
	ADOGRoom* RoomLeftIfValid = LevelLayoutBuilt[RoomPos(LevelLength - 1, LevelLength, LevelLength)];
	
	//Check RoomUp for connection
	ADOGRoom* RoomUpIfValid = LevelLayoutBuilt[RoomPos(LevelLength, LevelLength - 1, LevelLength)];

	//Check RoomRight for connection
	ADOGRoom* RoomRightIfValid = LevelLayoutBuilt[RoomPos(LevelLength + 1, LevelLength, LevelLength)];
	
	
	SpawnedRoom->RoomLeft = RoomLeftIfValid;
	SpawnedRoom->RoomUp = RoomUpIfValid;
	SpawnedRoom->RoomRight = RoomRightIfValid;
	SpawnedRoom->RoomType = ERoomType::RTMerchant;

	SpawnedRoom->FinishSpawning(Transform);

	SpawnedRoom->SetActorRotation(FRotator(0,270,0));
			
	SpawnedRoom->AttachToActor(this, FAttachmentTransformRules::KeepRelativeTransform);
	
	StartingRoom = LevelLayoutBuilt[RoomPos(LevelLength, LevelLength, LevelLength)];
	FTransform NewTransform = StartingRoom->GetTransform();
	StartingRoom->Destroy();
	SpawnedRoom->SetActorTransform(NewTransform);
	StartingRoom = SpawnedRoom;
	
	//light up start
	StartingRoom->SetActorHiddenInGame(false);
	StartingRoom->SetAllDoorsHidden(false);
	StartingRoom->SetActorTickEnabled(true);
	StartingRoom->RoomManager->SetActorTickEnabled(true);

	RoomLeftIfValid->RoomManager->RoomRight = SpawnedRoom;
	RoomLeftIfValid->RoomRight = SpawnedRoom;

	RoomUpIfValid->RoomManager->RoomDown = SpawnedRoom;
	RoomUpIfValid->RoomDown = SpawnedRoom;

	RoomRightIfValid->RoomManager->RoomLeft = SpawnedRoom;
	RoomRightIfValid->RoomLeft = SpawnedRoom;
}

int ADOGGenerator::RoomPos(int32 X, int32 Y, int32 Length)
{
	return X + Y * (2 * Length + 1);
}

UDOGRoomData* ADOGGenerator::GetValidRoom(int32 X, int32 Y, int32 Length)
{
	UDOGRoomData* NewRoom = nullptr;
	ERoomType TopLeftRoomType = ERoomType::RTEmpty;
	ERoomType TopRightRoomType = ERoomType::RTEmpty;
	ERoomType TwiceLeftRoomType = ERoomType::RTEmpty;


	int Middle = Y * (2 * Length + 1) + Length;
	int TopRange = Middle + Y;
	int BottomRange = Middle - Y;
	int RoomIndex = RoomPos(X, Y, Length);
	
	if(RoomIndex > TopRange || RoomIndex < BottomRange)
	{
		NewRoom = EmptyRooms[0];
	}
	else
	{
		bool bLeftOnceInRange = X - 1 > 0;
		bool bRightOnceInRange = X + 1 < 2 * Length + 1;
		bool bUpOnceInRange = Y - 1 > 0;
		bool bLeftTwiceInRange = X - 2 > 0 ;
		
		if (bLeftOnceInRange && bUpOnceInRange)
		{
			TopLeftRoomType = LevelLayout[RoomPos(X - 1, Y - 1, Length)]->RoomType;
		}
		if (bRightOnceInRange && bUpOnceInRange)
		{
			TopRightRoomType = LevelLayout[RoomPos(X + 1, Y - 1, Length)]->RoomType;
		}
		if (bLeftTwiceInRange)
		{
			TwiceLeftRoomType = LevelLayout[RoomPos(X - 2, Y, Length)]->RoomType;
		}

		while(
			!IsValid(NewRoom) ||
			NewRoom->RoomType == TopLeftRoomType ||
			NewRoom->RoomType == TopRightRoomType ||
			NewRoom->RoomType == TwiceLeftRoomType
			)
		{
			int SwitchType = FMath::RandRange(0, 3);
			int Index;
			switch (SwitchType)
			{
			case 0:
				Index = FMath::RandRange(0, MerchantRooms.Num() - 1);
				NewRoom = MerchantRooms[Index];
				break;
			case 1:
				Index = FMath::RandRange(0, GunRooms.Num() - 1);
				NewRoom = GunRooms[Index];
				break;
			case 2:
				Index = FMath::RandRange(0, UpgradeRooms.Num() - 1);
				NewRoom = UpgradeRooms[Index];
				break;
			case 3:
				Index = FMath::RandRange(0, ScrapRooms.Num() - 1);
				NewRoom = ScrapRooms[Index];
				break;
			default:
				break;
			}
		}
	}

	return NewRoom;
}

void ADOGGenerator::AddToRoomCorrectArray(UDOGRoomData* RoomData)
{
	if (!RoomData->bShouldBeGenerated)
	{
		return;
	}
	
	switch (RoomData->RoomType)
	{
	case RTMerchant:
		MerchantRooms.Add(RoomData);
		break;
	case RTGun:
		GunRooms.Add(RoomData);
		break;
	case RTUpgrade:
		UpgradeRooms.Add(RoomData);
		break;
	case RTScrap:
		ScrapRooms.Add(RoomData);
		break;
	case RTBoss:
		BossRooms.Add(RoomData);
		break;
	case RTEmpty:
		EmptyRooms.Add(RoomData);
		break;
	default:
		break;
	}
}

void ADOGGenerator::InitateBossSetup()
{
	//TODO
	//De-Spawn generated level
	for (auto Room : LevelLayoutBuilt)
	{
		if (IsValid(Room))
		{
			if (IsValid(Room->RoomManager))
			{
				Room->RoomManager->Destroy();
			}
			Room->Destroy();
		}
	}

	PreBossShop->SetActorLocation(Player->GetActorLocation());
	PreBossShop->RoomManager->EnteredRoom();
	PreBossShop->SetActorTickEnabled(true);
	PreBossShop->RoomManager->bShouldCallUpdate = true;

	//TP to boss and shop setup?
	//Player->SetActorLocation(Player->GetActorLocation() + FVector(0.f,0,500.f));
	
}

// Called when the game starts or when spawned
void ADOGGenerator::BeginPlay()
{
	Super::BeginPlay();

	if (!Cast<UDOGGameInstance>(UGameplayStatics::GetGameInstance(GetWorld()))->bTutorial)
	{
		bHasDoneTutorial = true;
	}
	
	UAssetManager* AssetManager = UAssetManager::GetIfValid();
	FPrimaryAssetType RoomDataAssets(FName("DOGRoomData"));
	TArray<FPrimaryAssetId> OutPrimaryAssetIdList;
	AssetManager->GetPrimaryAssetIdList(RoomDataAssets, OutPrimaryAssetIdList);

	AssetAmount = OutPrimaryAssetIdList.Num();

	for (auto PrimaryAssetId : OutPrimaryAssetIdList)
	{
		UObject* AssetObject = AssetManager->GetPrimaryAssetObject(PrimaryAssetId);
		if (IsValid(AssetObject))
		{
			UDOGRoomData* Room = Cast<UDOGRoomData>(AssetObject);

			AddToRoomCorrectArray(Room);
			AssetAmount--;
		}
		else
		{
			TArray<FName> Bundles;
			FStreamableDelegate Delegate = FStreamableDelegate::CreateUObject(this, &ADOGGenerator::OnRoomLoaded, PrimaryAssetId);
			AssetManager->LoadPrimaryAsset(PrimaryAssetId, Bundles, Delegate);
		}
	}
}

// Called every frame
void ADOGGenerator::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (!bHasDoneTutorial && IsValid(Player))
	{
		if (!Cast<UDOGGameInstance>(UGameplayStatics::GetGameInstance(GetWorld()))->bTutorial)
		{
			//TODO
			bHasDoneTutorial = true;

			Player->SetActorLocation(StartingRoom->GetActorLocation() + FVector(-1030.f,0,10.f));
		}
	}
	
	if (AssetAmount <= 0)
	{		
		if (!bLevelGenerated)
		{
			bLevelGenerated = true;
			GeneratedLength = FMath::RandRange(MinLength, MaxLength);
			GenerateLevelLayout(GeneratedLength);
			
			SetActorRotation(FRotator(0, 90.f, 0));

			for (auto Room : LevelLayoutBuilt)
			{
				Room->FinishedSpawning();	
			}
		}

		if (IsValid(GetWorld()) && !bPlayerIsSpawned && bLevelGenerated)
		{
			Player = UGameplayStatics::GetActorOfClass(GetWorld(), ADOGPlayer::StaticClass());
			if (IsValid(Player))
			{
				bPlayerIsSpawned = true;

				UNavigationSystemV1* Nav = UNavigationSystemV1::GetCurrent(GetWorld());
				Nav->OnNavigationBoundsUpdated(CastChecked<ANavMeshBoundsVolume>(UGameplayStatics::GetActorOfClass(GetWorld(), ANavMeshBoundsVolume::StaticClass())));

				if (Cast<UDOGGameInstance>(UGameplayStatics::GetGameInstance(GetWorld()))->bTutorial && IsValid(TutorialStartRoom))
				{
					Player->SetActorLocation(TutorialStartRoom->GetActorLocation() + FVector(-70.f,0,500.f));
				}
				else
				{
					Player->SetActorLocation(StartingRoom->GetActorLocation() + FVector(-70.f,0,500.f));
				}

				OnFinishedGenerating(true);
			}
			else
			{
				OnFinishedGenerating(false);
			}
		}
	}
}

