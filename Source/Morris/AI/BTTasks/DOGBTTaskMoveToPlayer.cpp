// Fill out your copyright notice in the Description page of Project Settings.


#include "DOGBTTaskMoveToPlayer.h"

#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Morris/AI/DOGAIController.h"
#include "Morris/AI/DOGEnemyBase.h"

UDOGBTTaskMoveToPlayer::UDOGBTTaskMoveToPlayer()
{
	NodeName = TEXT("Move To Player");

	bNotifyTick = true;
}

EBTNodeResult::Type UDOGBTTaskMoveToPlayer::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{   
	// Get Controller
	ADOGAIController* AIController = Cast<ADOGAIController>(OwnerComp.GetAIOwner());
	
	// Get the Pawn enemy base
	ADOGEnemyBase* AIPawn = AIController->GetPawn<ADOGEnemyBase>();

	// Checks if its valid / stops crashes
	if (!IsValid(AIPawn) || !IsValid(AIPawn->GetTarget()))
	{
		UE_LOG(LogTemp, Warning, TEXT("Move To Player failed"), *AIPawn->GetName());
		FinishLatentAbort(OwnerComp);
		return EBTNodeResult::Aborted;
	}

	// Move to player
	AIController->MoveToActor(AIPawn->GetTarget());

	return  EBTNodeResult::InProgress;
}

void UDOGBTTaskMoveToPlayer::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	// Get Controller
	ADOGAIController* AIController = Cast<ADOGAIController>(OwnerComp.GetAIOwner());
	
	// Get the Pawn enemy base
	ADOGEnemyBase* AIPawn = AIController->GetPawn<ADOGEnemyBase>();
	
	// Get stop from blueprint
	float AcceptanceRadius = AIPawn->FinishMoveToRange;

	if (AIController->GetMoveStatus() == EPathFollowingStatus::Idle)
		AIController->MoveToActor(AIPawn->GetTarget());
	
	// If the Blackboard key is not set it AcceptanceRadius becomes Range
	if (AcceptanceRadius <= 0 || NULL)
		AcceptanceRadius = Range;

	const float DisToPlayer = (AIPawn->GetTarget()->GetTransform().GetLocation() - AIPawn->GetTransform().GetLocation()).Length();
	
	if (AcceptanceRadius > DisToPlayer)
	{
		AIController->StopMovement();
		
		// Complete task
		FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
	}
}


FString UDOGBTTaskMoveToPlayer::GetStaticDescription() const
{
	return FString::Printf(TEXT("Takes Stop Range"));
} 
