// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ArrowComponent.h"
#include "Components/BoxComponent.h"
#include "Components/WidgetComponent.h"
#include "GameFramework/Actor.h"
#include "Morris/Data/DOGData.h"
#include "Morris/Generation/DOGRoomManager.h"
#include "DOGRoom.generated.h"

UCLASS(Abstract, Blueprintable)
class MORRIS_API ADOGRoom : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	ADOGRoom();
	
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category="Default")
	TObjectPtr<USceneComponent> DefaultSceneRoot;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Variables")
	TSubclassOf<ADOGRoomManager> RoomManagerClass = nullptr;
	
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category="Variables")
	ADOGRoomManager* RoomManager;

	/** Bounding box for the room */
	UPROPERTY(BlueprintReadWrite, VisibleAnywhere, Category="Variables")
	TObjectPtr<UBoxComponent> RoomSize;

	/** Bounding box for the room */
	UPROPERTY(BlueprintReadWrite, VisibleAnywhere, Category="Variables")
	TObjectPtr<UStaticMeshComponent> RoomMesh;
	
	/** Bounding box for the entry */
	UPROPERTY(BlueprintReadWrite, VisibleAnywhere, Category="Variables")
	TObjectPtr<UBoxComponent> RoomEntrySize;

	/** Arrow for spawning Top Door */
	UPROPERTY(BlueprintReadWrite, VisibleAnywhere, Category="Variables")
	TObjectPtr<UArrowComponent> ArrowDoorTop;

	/** Arrow for spawning Right Door */
	UPROPERTY(BlueprintReadWrite, VisibleAnywhere, Category="Variables")
	TObjectPtr<UArrowComponent> ArrowDoorRight;

	/** Arrow for spawning Left Door */
	UPROPERTY(BlueprintReadWrite, VisibleAnywhere, Category="Variables")
	TObjectPtr<UArrowComponent> ArrowDoorLeft;

	/** Arrow for spawning Bottom Door */
	UPROPERTY(BlueprintReadWrite, VisibleAnywhere, Category="Variables")
	TObjectPtr<UArrowComponent> ArrowDoorBottomNew;

	/** Door to spawn */
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Variables")
	TSubclassOf<ADOGDoor> DoorClass = nullptr;

	/** TArray for doors */
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Variables")
	TArray<ADOGDoor*> DoorArray;

	/** The Image used to represent this room or type of room */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(ExposeOnSpawn="true"))
	UTexture2D* IconImage; 

	/** The Image used to represent this room or type of room */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(ExposeOnSpawn="true"))
	UMaterialInstance* IconMaterial; 

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	UWidgetComponent* UpWidget = nullptr;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	UWidgetComponent* RightWidget = nullptr;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	UWidgetComponent* LeftWidget = nullptr;
	
	/** called when something enters the RoomSize component */
	UFUNCTION(BlueprintCallable)
	void OnOverlapBeginCheck(class AActor* OtherActor);	
	
	/** called when something leaves the RoomSize component */
	UFUNCTION(BlueprintCallable)
	void OnOverlapEndCheck(class AActor* OtherActor);


	UPROPERTY(BlueprintReadWrite, EditAnywhere, meta=(ExposeOnSpawn="true"))
	ADOGRoom* RoomUp = nullptr;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, meta=(ExposeOnSpawn="true"))
	ADOGRoom* RoomRight = nullptr;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, meta=(ExposeOnSpawn="true"))
	ADOGRoom* RoomLeft = nullptr;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, meta=(ExposeOnSpawn="true"))
	ADOGRoom* RoomDown = nullptr;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, meta=(ExposeOnSpawn="true"))
	TEnumAsByte<ERoomType> RoomType = RTEmpty;
	
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	virtual void OnConstruction(const FTransform& Transform) override;

	void SetEntranceToClosestDoor(FVector PlayerLoc);

	virtual void BeginDestroy() override;

	void UpdateNearbyVisibility();

	void HidePreviousRooms();

	void ShowNextRooms();

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	
	void SetAllDoorsHidden(bool bShouldBeHidden);

	UFUNCTION()
	void FinishedSpawning();

	UFUNCTION(BlueprintCallable)
	ADOGRoom* GetPreviousRoom();
};
