// Fill out your copyright notice in the Description page of Project Settings.


#include "DOGMovementComponent.h"

#include "GameplayTask.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/FloatingPawnMovement.h"


// Sets default values for this component's properties
UDOGMovementComponent::UDOGMovementComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}

// Called when the game starts
void UDOGMovementComponent::BeginPlay()
{
	Super::BeginPlay();
	
}


// Called every frame
void UDOGMovementComponent::TickComponent(float DeltaTime, ELevelTick TickType,
                                          FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	
	FHitResult Hit;
	FVector Start = GetOwner()->GetActorLocation() + GetOwner()->GetActorUpVector() * 90.0f;
	FVector End = GetOwner()->GetActorLocation() + GetOwner()->GetActorUpVector() * -91.0f;
	FCollisionQueryParams QueryParams;
	QueryParams.AddIgnoredActor(GetOwner());
	GetWorld()->LineTraceSingleByChannel(Hit, Start, End, ECC_Visibility, QueryParams);

	//Gravity
	if (Hit.bBlockingHit && IsValid(Hit.GetActor()))
	{
		GetOwner()->SetActorLocation(FVector(GetOwner()->GetActorLocation().X, GetOwner()->GetActorLocation().Y, Hit.Location.Z+90.f));
	}
	else
	{
		if(!(Velocity.Length() >= MaxSpeed))
		{
			AddInputVector(FVector(0,0,-1) * GravityMultiplier, false);
		}
	}
	
	// ...
}

