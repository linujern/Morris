// Fill out your copyright notice in the Description page of Project Settings.


#include "DOGBTTaskStrafe.h"

#include "NavigationSystem.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Morris/AI/DOGAIController.h"
#include "Morris/AI/EnemyTypes/DOGEnemyRanged.h"

UDOGBTTaskStrafe::UDOGBTTaskStrafe()
{
	NodeName = "Strafe";
    bNotifyTick = false;
	BlackboardKey.AddVectorFilter(this, GET_MEMBER_NAME_CHECKED(UDOGBTTaskStrafe, BlackboardKey));

}

EBTNodeResult::Type UDOGBTTaskStrafe::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	// Get the AI controller
    ADOGAIController* AIController = Cast<ADOGAIController>(OwnerComp.GetAIOwner());
	
    // Calculate the strafe direction
    FVector StrafeDirection = FVector::CrossProduct(AIController->GetPawn()->GetActorForwardVector(), FVector::UpVector);
	
    // Moves Random Direction
    if (UKismetMathLibrary::RandomBool())
        StrafeDirection = -StrafeDirection;
    
	UNavigationSystemV1* NavSystem = UNavigationSystemV1::GetCurrent(GetWorld());
	FVector TestLocation = AIController->GetPawn()->GetActorLocation() + StrafeDirection * AIController->GetPawn<ADOGEnemyBase>()->StrafeRange;
	FNavLocation NewLocation;
	NavSystem->ProjectPointToNavigation(TestLocation, NewLocation);
	Location = NewLocation.Location;
    
	AIController->GetBlackboardComponent()->SetValueAsVector(BlackboardKey.SelectedKeyName, NewLocation.Location);
	
    // Return a result indicating that the Task is still running
    return EBTNodeResult::Succeeded;
}

void UDOGBTTaskStrafe::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	//TODO: Clean tasks and enemies of things
}


FString UDOGBTTaskStrafe::GetStaticDescription() const
{
	return FString::Printf(TEXT("Ranged Enemy Strafe"));
}


