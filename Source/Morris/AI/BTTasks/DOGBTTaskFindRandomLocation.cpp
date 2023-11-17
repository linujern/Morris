// Fill out your copyright notice in the Description page of Project Settings.


#include "DOGBTTaskFindRandomLocation.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Morris/AI/DOGAIController.h"
#include "NavigationSystem.h"
#include "Morris/AI/DOGEnemyBase.h"

UDOGBTTaskFindRandomLocation::UDOGBTTaskFindRandomLocation()
{
	NodeName = TEXT("Find Random Location");

	BlackboardKey.AddVectorFilter(this, GET_MEMBER_NAME_CHECKED(UDOGBTTaskFindRandomLocation, BlackboardKey));
}

EBTNodeResult::Type UDOGBTTaskFindRandomLocation::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	FNavLocation Location;

	ADOGAIController* AIController = Cast<ADOGAIController>(OwnerComp.GetAIOwner());
	
	// Anti-Crash
	if (!IsValid(AIController))
	{
		UE_LOG(LogTemp, Warning, TEXT("AIController not vaild - 1.1 B"));
		FinishLatentAbort(OwnerComp);
		return EBTNodeResult::Aborted;
	}

	if (!IsValid(AIController->GetPawn()))
	{
		UE_LOG(LogTemp, Warning, TEXT("AIController not vaild - 1.1 B"));
		FinishLatentAbort(OwnerComp);
		return EBTNodeResult::Aborted;
	}
	
	UNavigationSystemV1* NavSystem = UNavigationSystemV1::GetCurrent(GetWorld());

	// Range of the random search
	float Radius = AIController->GetPawn<ADOGEnemyBase>()->RandomMoveRange;

	if (Radius == 0)
		Radius = SearchRadius;
	

	if (IsValid(NavSystem) && NavSystem->GetRandomPointInNavigableRadius(AIController->GetPawn()->GetTransform().GetLocation(), Radius, Location) )
		AIController->GetBlackboardComponent()->SetValueAsVector(BlackboardKey.SelectedKeyName, Location.Location);
	

	FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
	return EBTNodeResult::Succeeded;
}

FString UDOGBTTaskFindRandomLocation::GetStaticDescription() const
{
	return FString::Printf(TEXT("Vector: %s"), *BlackboardKey.SelectedKeyName.ToString());
}

