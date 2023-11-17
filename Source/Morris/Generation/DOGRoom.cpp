// Fill out your copyright notice in the Description page of Project Settings.


#include "DOGRoom.h"

#include "Morris/Player/DOGPlayer.h"


// Sets default values
ADOGRoom::ADOGRoom()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
	
	//Create components
	DefaultSceneRoot = CreateDefaultSubobject<USceneComponent>(TEXT("SceneRoot"));
	RootComponent = DefaultSceneRoot;
	
	RoomMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("RoomMesh"));
	RoomMesh->SetupAttachment(RootComponent);
	
	RoomSize = CreateDefaultSubobject<UBoxComponent>(TEXT("RoomSizeBox"));
	RoomSize->SetupAttachment(RootComponent);

	RoomSize->SetCollisionProfileName(TEXT("OverlapAllDynamic"));

	RoomEntrySize = CreateDefaultSubobject<UBoxComponent>(TEXT("RoomEntrySizeBox"));
	
	RoomEntrySize->SetCollisionProfileName(TEXT("OverlapAllDynamic"));
	
	ArrowDoorTop	= CreateDefaultSubobject<UArrowComponent>(TEXT("DoorTopArrow"));
	ArrowDoorRight	= CreateDefaultSubobject<UArrowComponent>(TEXT("DoorRightArrow"));
	ArrowDoorLeft	= CreateDefaultSubobject<UArrowComponent>(TEXT("DoorLeftArrow"));
	ArrowDoorBottomNew	= CreateDefaultSubobject<UArrowComponent>(TEXT("DoorBottomArrowNew"));
	
	ArrowDoorTop	->SetupAttachment(DefaultSceneRoot);
	ArrowDoorRight	->SetupAttachment(DefaultSceneRoot);
	ArrowDoorLeft	->SetupAttachment(DefaultSceneRoot);
	ArrowDoorBottomNew	->SetupAttachment(DefaultSceneRoot);
	RoomEntrySize	->SetupAttachment(RoomSize);

	UpWidget = CreateDefaultSubobject<UWidgetComponent>(TEXT("UpWidget"));
	UpWidget->SetupAttachment(ArrowDoorTop);
	RightWidget = CreateDefaultSubobject<UWidgetComponent>(TEXT("RightWidget"));
	RightWidget->SetupAttachment(ArrowDoorRight);
	LeftWidget = CreateDefaultSubobject<UWidgetComponent>(TEXT("LeftWidget"));
	LeftWidget->SetupAttachment(ArrowDoorLeft);
	
	//Enter & Leave
	//RoomSize->OnComponentBeginOverlap.AddDynamic(this, &ADOGRoom::OnOverlapBegin); // set up a notification for when this component overlaps something
	//RoomSize->OnComponentEndOverlap.AddDynamic(this, &ADOGRoom::OnOverlapEnd); // set up a notification for when this component overlaps something
}

void ADOGRoom::OnOverlapBeginCheck(class AActor* OtherActor)
{
	if (OtherActor->IsA(ADOGPlayer::StaticClass()))
	{
		//UKismetSystemLibrary::PrintString(GetWorld(), "we ball");
		SetEntranceToClosestDoor(OtherActor->GetActorLocation());
		RoomManager->EnteredRoom();
		UpdateNearbyVisibility();
	}
}

void ADOGRoom::OnOverlapEndCheck(AActor* OtherActor)
{
	if (OtherActor->IsA(ADOGPlayer::StaticClass()))
	{
		//UKismetSystemLibrary::PrintString(GetWorld(), "we dont ball");
		RoomManager->ExitedRoom();
	}
}

// Called when the game starts or when spawned
void ADOGRoom::BeginPlay()
{
	Super::BeginPlay();

	//dont think this works, kept just in case
	// const FTransform SpawnLocAndRotation = GetActorTransform();
	// ADOGRoomManager* Manager = GetWorld()->SpawnActorDeferred<ADOGRoomManager>(ADOGRoomManager::StaticClass(), SpawnLocAndRotation);
	// Manager->FinishSpawning(SpawnLocAndRotation);

	//make manager
	if (RoomManagerClass)
	{
		RoomManager = GetWorld()->SpawnActor<ADOGRoomManager>(RoomManagerClass, this->GetTransform());
		RoomManager->RoomCurrent = this;
		RoomManager->AttachToActor(this, FAttachmentTransformRules::SnapToTargetNotIncludingScale);

		RoomManager->SetActorTickEnabled(false);
	}

	//set rooms
	if (RoomUp) RoomManager->RoomUp = RoomUp;
	if (RoomRight) RoomManager->RoomRight = RoomRight;
	if (RoomLeft) RoomManager->RoomLeft = RoomLeft;
	if (RoomDown) RoomManager->RoomDown = RoomDown;

	
	//Spawn doors
	if (DoorClass)
	{
		ADOGDoor* DoorTop = GetWorld()->SpawnActor<ADOGDoor>(DoorClass, ArrowDoorTop->GetComponentTransform());
		DoorTop->AttachToActor(this, FAttachmentTransformRules::KeepWorldTransform, NAME_None);
		DoorTop->bIsOpen = true;
		DoorTop->OnDoorOpen();
		DoorArray.Add(DoorTop);
		
		ADOGDoor* DoorRight = GetWorld()->SpawnActor<ADOGDoor>(DoorClass, ArrowDoorRight->GetComponentTransform());
		DoorRight->AttachToActor(this, FAttachmentTransformRules::KeepWorldTransform, NAME_None);
		DoorRight->bIsOpen = true;
		DoorRight->OnDoorOpen();
		DoorArray.Add(DoorRight);
	
		ADOGDoor* DoorLeft = GetWorld()->SpawnActor<ADOGDoor>(DoorClass, ArrowDoorLeft->GetComponentTransform());
		DoorLeft->AttachToActor(this, FAttachmentTransformRules::KeepWorldTransform, NAME_None);
		DoorLeft->bIsOpen = true;
		DoorLeft->OnDoorOpen();
		DoorArray.Add(DoorLeft);
	
		ADOGDoor* DoorBottom = GetWorld()->SpawnActor<ADOGDoor>(DoorClass, ArrowDoorBottomNew->GetComponentTransform());
		DoorBottom->AttachToActor(this, FAttachmentTransformRules::KeepWorldTransform, NAME_None);
		DoorBottom->bIsOpen = true;
		DoorBottom->OnDoorOpen();
		DoorArray.Add(DoorBottom);

		//give manager the doors
		RoomManager->SetupDoors(DoorTop, DoorRight, DoorLeft, DoorBottom);
	}
}

void ADOGRoom::OnConstruction(const FTransform& Transform)
{
	Super::OnConstruction(Transform);

	ArrowDoorTop	->RegisterComponent();
	ArrowDoorRight	->RegisterComponent();
	ArrowDoorLeft	->RegisterComponent();
	ArrowDoorBottomNew	->RegisterComponent();

}

void ADOGRoom::SetEntranceToClosestDoor(FVector PlayerLoc)
{
	TArray<AActor*> LocalDoorArray;
	for (auto Door : DoorArray)
	{
		LocalDoorArray.Add(Door);
	}
	float outDistance;
	AActor* NearestActor = UGameplayStatics::FindNearestActor(PlayerLoc, LocalDoorArray, outDistance);
	ADOGDoor* NearestDoor = Cast<ADOGDoor>(NearestActor);
	NearestDoor->bIsEntrance = true;
}

void ADOGRoom::BeginDestroy()
{
	Super::BeginDestroy();
	
	for(auto Door : DoorArray)
	{
		Door->Destroy();
	}
	DoorArray.Empty();
}

void ADOGRoom::UpdateNearbyVisibility()
{
	ShowNextRooms();

	if (!GetPreviousRoom())
	{
		return;
	}

	HidePreviousRooms();
}

void ADOGRoom::HidePreviousRooms()
{
	//Hide previous and related
	ADOGRoomManager* PrevMngr = GetPreviousRoom()->RoomManager;
	TArray<ADOGRoom*> TempRoomArray;

	TempRoomArray.Add(GetPreviousRoom());
	if (PrevMngr->RoomUp)
	{
		TempRoomArray.Add(PrevMngr->RoomUp);
	}
	if (PrevMngr->RoomLeft)
	{
		TempRoomArray.Add(PrevMngr->RoomLeft);
	}
	if (PrevMngr->RoomRight)
	{
		TempRoomArray.Add(PrevMngr->RoomRight);
	}
	
	for (auto Room : TempRoomArray)
	{
		if (Room && Room != this)
		{
			Room->SetActorHiddenInGame(true);
			Room->SetAllDoorsHidden(true);
			Room->SetActorTickEnabled(false);
			//Room->RoomManager->SetActorTickEnabled(false);
		}
	}
}

void ADOGRoom::ShowNextRooms()
{
	TArray<ADOGRoom*> TempRoomArray;
	
	//Show upcoming rooms
	TempRoomArray.Empty();
	
	if (this->RoomUp)
	{
		TempRoomArray.Add(this->RoomUp);
	}
	if (this->RoomLeft)
	{
		TempRoomArray.Add(this->RoomLeft);
	}
	if (this->RoomRight)
	{
		TempRoomArray.Add(this->RoomRight);
	}
	
	for (auto Room : TempRoomArray)
	{
		Room->SetActorHiddenInGame(false);
		Room->SetAllDoorsHidden(false);
		Room->SetActorTickEnabled(true);
		//Room->RoomManager->SetActorTickEnabled(true);
	}
}


// Called every frame
void ADOGRoom::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ADOGRoom::SetAllDoorsHidden(bool bShouldBeHidden)
{
	for (auto Door : DoorArray)
	{
		Door->SetActorHiddenInGame(bShouldBeHidden);
		Door->SetActorEnableCollision(!bShouldBeHidden);
		Door->SetActorTickEnabled(!bShouldBeHidden);
	}
}

void ADOGRoom::FinishedSpawning()
{
	for (auto Door : DoorArray)
	{
		Door->OnFinishSpawning();
	}
}

ADOGRoom* ADOGRoom::GetPreviousRoom()
{
	ADOGRoom* OutRoom = nullptr;
		
	for (int i = 0; i <= 3; i++)
	{
		if (DoorArray[i]->bIsEntrance)
		{
			switch (i)
			{
			case 0:
				//Top
				OutRoom = RoomManager->RoomUp;
				break;
			case 1:
				//Right
				OutRoom = RoomManager->RoomRight;
				break;
			case 2:
				//Left
				OutRoom = RoomManager->RoomLeft;
				break;
			case 3:
				//Bottom
				OutRoom = RoomManager->RoomDown;
				break;
			default:
				break;
			}
			break;
		}
	}
	return OutRoom;
}
