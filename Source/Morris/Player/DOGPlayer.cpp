// Fill out your copyright notice in the Description page of Project Settings.


#include "DOGPlayer.h"

#include "CollisionDebugDrawingPublic.h"
#include "DOGPlayerCamera.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/Character.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"
#include "Morris/DOGGameInstance.h"
#include "Morris/Interactables/DOGPowerPickup.h"
#include "Morris/Interactables/DOGPowerup.h"
#include "Morris/Interactables/DOGWeapon.h"
#include "Morris/Interactables/DOGWeaponPickup.h"
#include "Morris/Interactables/DOGWorldInteractable.h"
#include "Morris/Interactables/Money/DOGScrapMetal.h"


// Sets default values
ADOGPlayer::ADOGPlayer()
{
	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Owner = UGameplayStatics::GetPlayerController(GetWorld(), 0);
	
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	CapsuleComponent = CreateDefaultSubobject<UCapsuleComponent>("Capsule Collider");
	CapsuleComponent->SetCapsuleHalfHeight(88.f);
	CapsuleComponent->SetCapsuleRadius(44.f);
	CapsuleComponent->SetGenerateOverlapEvents(true);
	CapsuleComponent->SetCollisionProfileName(TEXT("BlockAllDynamic"));
	RootComponent = CapsuleComponent;

	MeshComponent = CreateDefaultSubobject<USkeletalMeshComponent>("Mesh");
	MeshComponent->SetupAttachment(CapsuleComponent);
	//MeshComponent->RegisterComponent();

	ArrowComponent = CreateDefaultSubobject<UArrowComponent>("SecondaryEquipmentLocation");
	ArrowComponent->SetupAttachment(MeshComponent, TEXT("QuickRigCharacter_Spine"));
	//ArrowComponent->RegisterComponent();

	HandComponent = CreateDefaultSubobject<UArrowComponent>("HandLocation");
	HandComponent->SetupAttachment(MeshComponent, TEXT("QuickRigCharacter_RightHand"));
	//HandComponent->RegisterComponent();

	
		MovementComponent = CreateDefaultSubobject<UDOGMovementComponent>("DOGMovement");
		
	
	
	if(MovementComponent)
		MovementComponent->SetUpdatedComponent(RootComponent);
	//AddOwnedComponent(MovementComponent);
	//MovementComponent->RegisterComponent();
}

void ADOGPlayer::DashReplenish()
{
	CurrentDash = CurrentDash + 1.f;

	DashTimerHandle.Invalidate();
	
	if(CurrentDash < MaxDash)
	{
		if(!DashTimerHandle.IsValid())
			GetWorld()->GetTimerManager().SetTimer(DashTimerHandle, this, &ADOGPlayer::DashReplenish, DashTimer, false);
	}

	if(HudWidget) HudWidget->OnUpdatedDashes(CurrentDash);
}

void ADOGPlayer::CheckForWorldInteractable()
{
	//Scrap
	TryPickupScrap();
	
	//World interactable
	TArray<TEnumAsByte<EObjectTypeQuery>> ObjectType;
	ObjectType.Add(UEngineTypes::ConvertToObjectType(ECC_WorldDynamic));
	UClass* ClassFilter = ADOGWorldInteractable::StaticClass();
	TArray<AActor*> IgnoredActors;
	
	TArray<AActor*> OutActors;
	
	UKismetSystemLibrary::SphereOverlapActors(GetWorld(), GetActorLocation(), InteractionRadius * 2, ObjectType, ClassFilter, IgnoredActors, OutActors);
	
	float Distance = 0.f;
	AActor* Nearest = UGameplayStatics::FindNearestActor(GetActorLocation(), OutActors, Distance);

	for (AActor* WI : OutActors)
	{
		if(IsValid(WI))
		{
			if(WI != Nearest) {
				ADOGWorldInteractable* WorldInteractable = Cast<ADOGWorldInteractable>(WI);
				WorldInteractable->ToggleOutline(false);
				WorldInteractable->NotInRangeOfItem();
			}	
		}
	}

	if(OutActors.Num() <= 0)
	{
		CurrentInteractableWorld = nullptr;
		return;
	}

	if(Distance <= InteractionRadius)
	{
		if(!IsValid(Nearest)) return;
		if (!Cast<ADOGWorldInteractable>(Nearest)->bCanInteract)
		{
			CurrentInteractableWorld = nullptr;
			Cast<ADOGWorldInteractable>(Nearest)->ToggleOutline(false);

			// For Shop UI
			if (Item)
				Item = Item->NotInRangeOfItem();
			return;
		}

		
		CurrentInteractableWorld = Cast<ADOGWorldInteractable>(Nearest);
		CurrentInteractableWorld->ToggleOutline(true);
		// For Shop UI
		if (CurrentInteractableWorld->HudWidget)
		{
			// Show UI
			Item = CurrentInteractableWorld->InRangeOfItem();

			if (!IsValid(Item->HudWidget)) return;

			Item->SetItemUI();
			
			FVector2d ScreenVector;
			FVector2d PlayerScreenVector;
			UGameplayStatics::ProjectWorldToScreen(Cast<APlayerController>(Owner), Item->GetTransform().GetLocation(), ScreenVector);
			UGameplayStatics::ProjectWorldToScreen(Cast<APlayerController>(Owner), this->GetTransform().GetLocation(), PlayerScreenVector);
			const FVector2d Scale (1,1);

			ScreenVector -= PlayerScreenVector;
			int Offset = 450.f;
			if (ScreenVector.X < 0)
				Offset = -Offset;
			ScreenVector.X += Offset;
			
			const FWidgetTransform WidgetTransform = FWidgetTransform(ScreenVector, Scale, FVector2d(0,0), 0);
			Item->HudWidget->SetRenderTransform(WidgetTransform);
			
		}
		else
		{
			if (Item)
				Item = Item->NotInRangeOfItem();
		}
		return;
	}
	
	if (IsValid(Nearest))
	{
		Cast<ADOGWorldInteractable>(Nearest)->ToggleOutline(false);
		if (Item)
			Item = Item->NotInRangeOfItem();
	}
	
	CurrentInteractableWorld = nullptr;
}

void ADOGPlayer::TryInteractWorld()
{
	if (bIsDead) return;

	if(CurrentInteractableWorld)
	{
		if (Item)
			Item->NotInRangeOfItem();
		CurrentInteractableWorld->Interact(this);
	}
}

void ADOGPlayer::TryInteractLocal()
{
	if (bIsDead) return;

	if(IsValid(PrimaryEquipment))
		PrimaryEquipment->Interact(this);
}

void ADOGPlayer::StartInteractLocal()
{
	bIsInteractingLocal = true;
}

void ADOGPlayer::EndInteractLocal()
{
	bIsInteractingLocal = false;
	if (HudWidget)
		HudWidget->OnInteractLocal(PrimaryEquipment);
}

void ADOGPlayer::TryReload()
{
	if (bIsDead) return;

	if(IsValid(PrimaryEquipment))
	{
		if(ADOGWeapon* Weapon = Cast<ADOGWeapon>(PrimaryEquipment))
			Weapon->StartReload();
	}
}

void ADOGPlayer::TryPickupScrap()
{
	if (bIsDead) return;

	TArray<TEnumAsByte<EObjectTypeQuery>> ObjectType;
	ObjectType.Add(UEngineTypes::ConvertToObjectType(ECC_WorldDynamic));
	UClass* ClassFilter = ADOGScrapMetal::StaticClass();
	TArray<AActor*> IgnoredActors;
	
	TArray<AActor*> OutActors;
	
	UKismetSystemLibrary::SphereOverlapActors(GetWorld(), GetActorLocation(), PickupScrapMetalRadius, ObjectType, ClassFilter, IgnoredActors, OutActors);

	for (AActor* Scrap : OutActors)
	{
		if(IsValid(Scrap))
		{
			Cast<ADOGScrapMetal>(Scrap)->TryCollect(this);
		}
	}
}

void ADOGPlayer::ApplyDamage(float Value)
{
	if(bIsDead) return;
	OnDamage();
	Health = Health - Value;
	HudWidget->HealthProgressBar->SetPercent(Health / MaxHealth);
	if(Health<=0)
	{
		OnDeath();
		bIsDead = true;
		MeshComponent->SetSkeletalMesh(DeathMesh);
		Health = 0;
		if (IsValid(PrimaryEquipment)) PrimaryEquipment->MeshComponent->SetVisibility(false);
		if (IsValid(SecondaryEquipment)) SecondaryEquipment->MeshComponent->SetVisibility(false);
		
		DisableInput(UGameplayStatics::GetPlayerController(GetWorld(), 0));
		UGameplayStatics::SetGlobalTimeDilation(GetWorld(), GameOverTimeDialation);
		
		GetWorld()->GetTimerManager().SetTimer(ResetGameHandle, this, &ADOGPlayer::ResetGame, FadeOutResetTime);
	}
	else if (Health > MaxHealth)
	{
		Health = MaxHealth;
	}
}

void ADOGPlayer::DealDamageToPlayer(float Value)
{
	ApplyDamage(Value);
}

void ADOGPlayer::SwapHands()
{
	if(bIsDead) return;

	//if(Cast<ADOGWeapon>(PrimaryEquipment))
	//	Cast<ADOGWeapon>(PrimaryEquipment)->TryCancelReload();
	ADOGInteractable* Item1 = PrimaryEquipment;
	ADOGInteractable* Item2 = SecondaryEquipment;
	PrimaryEquipment = Item2;
	SecondaryEquipment = Item1;
	
	if(IsValid(PrimaryEquipment))
	{
		PrimaryEquipment->DetachFromActor(FDetachmentTransformRules(EDetachmentRule::KeepWorld, false));
		PrimaryEquipment->AttachToComponent(HandComponent, FAttachmentTransformRules(EAttachmentRule::SnapToTarget, false));
	}

	if(IsValid(SecondaryEquipment))
	{
		SecondaryEquipment->DetachFromActor(FDetachmentTransformRules(EDetachmentRule::KeepWorld, false));
		SecondaryEquipment->AttachToComponent(ArrowComponent, FAttachmentTransformRules(EAttachmentRule::SnapToTarget, false));
	}
	
	HudWidget->OnSwapEquipment(PrimaryEquipment, SecondaryEquipment);
}

void ADOGPlayer::TryDropHand()
{
	if (bIsDead) return;

	if(PrimaryEquipment != nullptr && IsValid(PrimaryEquipment))
	{
		//TODO: Code this
			FTransform Transform;

		if (ADOGWeapon* Weapon = Cast<ADOGWeapon>(PrimaryEquipment))
		{
			Transform.SetLocation(GetActorLocation() + MeshComponent->GetRightVector() * DropDistance);
			AActor* Pickup = GetWorld()->SpawnActorDeferred<ADOGWeaponPickup>(WeaponPickupClass, Transform);
			ADOGWeaponPickup* WeaponPickup = Cast<ADOGWeaponPickup>(Pickup);
			WeaponPickup->WeaponData = Weapon->WeaponData;
			Pickup->FinishSpawning(Transform);

			WeaponPickup->MeshComponent->SetSimulatePhysics(true);
			WeaponPickup->MeshComponent->AddImpulse(MeshComponent->GetRightVector() * DropForce, NAME_None, true);
			
			Weapon->Destroy();
		}
		else if (PrimaryEquipment->IsA(ADOGPowerup::StaticClass()))
		{
			ADOGPowerup* Powerup = Cast<ADOGPowerup>(PrimaryEquipment);
			Transform.SetLocation(GetActorLocation() + MeshComponent->GetRightVector() * DropDistance);
			AActor* Pickup = GetWorld()->SpawnActorDeferred<ADOGPowerPickup>(PowerPickupClass, Transform);
			ADOGPowerPickup* PowerPickup = Cast<ADOGPowerPickup>(Pickup);
			PowerPickup->PowerPickupData = Powerup->PowerupData;
			Pickup->FinishSpawning(Transform);

			PowerPickup->MeshComponent->SetSimulatePhysics(true);
			PowerPickup->MeshComponent->AddImpulse(MeshComponent->GetRightVector() * DropForce, NAME_None, true);

			Powerup->Destroy();
		}
		
		HudWidget->OnDropHeldEquipment();
	}
}

void ADOGPlayer::Dash()
{
	if (bIsDead) return;

	if(CurrentDash <= 0)
		return;
	
	FVector Forward = GetVelocity();
	Forward.Normalize();

	if(Forward.Length() < 0.1f)
	{
		//Right is forward due to gamepad issue
		Forward = MeshComponent->GetRightVector();
	}

	FVector Start = GetActorLocation();
	FVector End = GetActorLocation() + Forward * DashLength;
	
	TArray<AActor*> Ignored;
	Ignored.Add(this);

	FHitResult Hit;
	
	if(UKismetSystemLibrary::LineTraceSingle(GetWorld(), Start, End, TraceTypeQuery1, true, Ignored, EDrawDebugTrace::None, Hit, true))
	{
		SetActorLocation(Hit.Location + Forward * -45.f);
		OnDash(Start, End);
	}
	else
	{
		SetActorLocation(End);
		OnDash(Start, End);
	}

	CurrentDash = CurrentDash - 1.f;

	if(HudWidget) HudWidget->OnUpdatedDashes(CurrentDash);

	if(!DashTimerHandle.IsValid())
		GetWorld()->GetTimerManager().SetTimer(DashTimerHandle, this, &ADOGPlayer::DashReplenish, DashTimer, false);
}

void ADOGPlayer::ResetGame()
{
	UGameplayStatics::OpenLevel(GetWorld(), "L_Game");
}

void ADOGPlayer::SaveScrapMetal()
{
	Cast<UDOGGameInstance>(UGameplayStatics::GetGameInstance(this))->ScrapMetal = ScrapMetal;
}

// Called when the game starts or when spawned
void ADOGPlayer::BeginPlay()
{
	Super::BeginPlay();
	
	EnableInput(UGameplayStatics::GetPlayerController(GetWorld(), 0));
	AActor* Camera = UGameplayStatics::GetActorOfClass(GetWorld(), ADOGPlayerCamera::StaticClass());
	if(IsValid(Camera))
	{
		Cast<ADOGPlayerCamera>(Camera)->Player = this;
		UGameplayStatics::GetPlayerController(GetWorld(), 0)->SetViewTarget(Camera);
	}
	APlayerController* PC = UGameplayStatics::GetPlayerController(GetWorld(), 0);
	FInputModeGameOnly InputMode;
	InputMode.SetConsumeCaptureMouseDown(false);
	PC->SetInputMode(InputMode);
	PC->SetShowMouseCursor(true);

	if (StartWeapon && !IsValid(PrimaryEquipment))
	{
		const FTransform SpawnLocAndRotation = GetActorTransform();
		ADOGWeapon* Weapon = GetWorld()->SpawnActorDeferred<ADOGWeapon>(ADOGWeapon::StaticClass(), SpawnLocAndRotation);
		Weapon->WeaponData = StartWeapon;
		Weapon->FinishSpawning(SpawnLocAndRotation);
		
		Weapon->AttachToComponent(HandComponent, FAttachmentTransformRules(EAttachmentRule::SnapToTarget, false));

		PrimaryEquipment = Weapon;
	}
	
	if(HudWidgetClass != nullptr)
	{
		UUserWidget* Widget = CreateWidget(GetWorld(), HudWidgetClass);
		HudWidget = Cast<UDOGHud>(Widget);
		HudWidget->HealthProgressBar->SetPercent(1.f);
		const FText ScrapAmount = FText::FromString(FString::FromInt(floor(ScrapMetal)));
		HudWidget->ScrapMetalAmount->SetText(ScrapAmount);
		HudWidget->AddToViewport();
		HudWidget->PlayerReference = this;
	}
		
	ScrapMetal = Cast<UDOGGameInstance>(UGameplayStatics::GetGameInstance(this))->ScrapMetal;
}

// Called every frame
void ADOGPlayer::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	CheckForWorldInteractable();

	// Cursed code 
	APlayerController* PC = UGameplayStatics::GetPlayerController(GetWorld(), 0);
	if(bIsInGameMouse)
	{
		FInputModeGameOnly InputMode;
		InputMode.SetConsumeCaptureMouseDown(false);
		PC->SetInputMode(InputMode);
	}
	
	if (bIsDead) return;

	if(bIsInteractingLocal) TryInteractLocal();

	//Movement
	if(bIsUsingGamepad)
	{
		PC->SetShowMouseCursor(false);
		
		if(TurnAtDirection.X > 0.5 || TurnAtDirection.Y > 0.5 || TurnAtDirection.X < -0.5 || TurnAtDirection.Y < -0.5)
		{
			FRotator LookAt = UKismetMathLibrary::FindLookAtRotation(
			MeshComponent->GetComponentLocation(),
			MeshComponent->GetComponentLocation() + TurnAtDirection*100);
	
			MeshComponent->SetWorldRotation(FRotator(MeshComponent->GetComponentRotation().Pitch, LookAt.Yaw, MeshComponent->GetComponentRotation().Roll));

			//UKismetSystemLibrary::PrintString(GetWorld(), "Rate: " + );	
		}
	}
	else
	{
		PC->SetShowMouseCursor(true);
		
		float LocationX;
		float LocationY;
		
		PC->GetMousePosition(LocationX, LocationY);

		const FVector2D MousePosition (LocationX, LocationY); 

		if (GEngine && GEngine->GameViewport)
		{
			FVector2D ScreenSize;
			
			GEngine->GameViewport->GetViewportSize(ScreenSize);

			ScreenSize = ScreenSize * 0.5f;

			const FRotator NewRotation = UKismetMathLibrary::FindLookAtRotation(FVector(ScreenSize.X, ScreenSize.Y, 0), FVector(MousePosition.X, MousePosition.Y, 0));

			PlayerRotator = NewRotation;

			FRotator Rotator = MeshComponent->GetComponentRotation();
			Rotator.Yaw = NewRotation.Yaw;
		
			MeshComponent->SetWorldRotation(Rotator);
		}
	}

	//HUD
	if(HudWidget)
	{
		if(ADOGWeapon* Weapon = Cast<ADOGWeapon>(PrimaryEquipment))
		{
			HudWidget->AmmoCountPrimary->SetText(FText::FromString(FString::FromInt(floor(Weapon->CurrentAmmo)) + "/" + FString::FromInt(floor(Weapon->WeaponData->BaseAmmo))));
		}
		else
		{
			HudWidget->AmmoCountPrimary->SetText(FText::FromString(""));
		}

		if(ADOGWeapon* Weapon = Cast<ADOGWeapon>(SecondaryEquipment))
		{
			HudWidget->AmmoCountSecondary->SetText(FText::FromString(FString::FromInt(floor(Weapon->CurrentAmmo)) + "/" + FString::FromInt(floor(Weapon->WeaponData->BaseAmmo))));
		}
		else
		{
			HudWidget->AmmoCountSecondary->SetText(FText::FromString(""));
		}
		HudWidget->ScrapMetalAmount->SetText(FText::FromString(FString::FromInt(floor(ScrapMetal))));
	}
}

// Called to bind functionality to input
void ADOGPlayer::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	check(PlayerInputComponent);

	PlayerInputComponent->BindAction("InteractWorld", IE_Pressed, this, &ADOGPlayer::TryInteractWorld);
	
	PlayerInputComponent->BindAction("InteractLocal", IE_Pressed, this, &ADOGPlayer::StartInteractLocal);
	PlayerInputComponent->BindAction("InteractLocal", IE_Released, this, &ADOGPlayer::EndInteractLocal);

	PlayerInputComponent->BindAction("Reload", IE_Pressed, this, &ADOGPlayer::TryReload);

	PlayerInputComponent->BindAction("Dash", IE_Pressed, this, &ADOGPlayer::Dash);

	PlayerInputComponent->BindAction("SwapItems", IE_Pressed, this, &ADOGPlayer::SwapHands);

	PlayerInputComponent->BindAction("AnyKey", IE_Released, this, &ADOGPlayer::AnyKey);
	
	PlayerInputComponent->BindAxis("MoveForwardBack", this, &ADOGPlayer::MoveRight);
	PlayerInputComponent->BindAxis("MoveRightLeft", this, &ADOGPlayer::MoveForward);

	//Look
	//PlayerInputComponent->BindAxis("TurnRightLeftMouse", this, &ADOGPlayer::TurnRightLeftGamepad);
	//PlayerInputComponent->BindAxis("TurnUpDownMouse", this, &ADOGPlayer::TurnUpDownGamepad);
	PlayerInputComponent->BindAxis("TurnRightLeftGamepad", this, &ADOGPlayer::TurnRightLeftGamepad);
	PlayerInputComponent->BindAxis("TurnUpDownGamepad", this, &ADOGPlayer::TurnUpDownGamepad);
}

void ADOGPlayer::AnyKey(FKey Key)
{
	//UKismetSystemLibrary::PrintString(GetWorld(), UKismetStringLibrary::Conv_BoolToString(Key.IsGamepadKey()));
	bIsUsingGamepad = Key.IsGamepadKey();
}

void ADOGPlayer::TurnRightLeftGamepad(float Rate)
{
	if ((Controller != nullptr) && (Rate != 0.0f))
	{
		TurnAtDirection.X = Rate;
	}
}

void ADOGPlayer::TurnUpDownGamepad(float Rate)
{
	if ((Controller != nullptr) && (Rate != 0.0f))
	{
		TurnAtDirection.Y = Rate;
	}
}

void ADOGPlayer::LookUpAtRate(float Rate)
{
	AddControllerPitchInput(Rate * GamepadTurnRate * GetWorld()->GetDeltaSeconds());
}

void ADOGPlayer::MoveForward(float Value)
{
	if ((Controller != nullptr) && (Value != 0.0f))
	{
		// find out which way is forward
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		// get forward vector
		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);
		AddMovementInput(Direction, Value);
	}
}

void ADOGPlayer::MoveRight(float Value)
{
	if ( (Controller != nullptr) && (Value != 0.0f) )
	{
		// find out which way is right
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);
	
		// get right vector 
		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
		// add movement in that direction
		AddMovementInput(Direction, Value);
	}
}

