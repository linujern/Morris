// Fill out your copyright notice in the Description page of Project Settings.


#include "DOGBTTaskBackAwayFromPlayer.h"

#include "NavigationSystem.h"
#include "Morris/AI/DOGAIController.h"
#include "Morris/AI/DOGEnemyBase.h"
#include "Morris/AI/EnemyTypes/DOGEnemyRanged.h"

class ADOGEnemyRanged;

UDOGBTTaskBackAwayFromPlayer::UDOGBTTaskBackAwayFromPlayer()
{
	NodeName = "Back Away From Player";

	//BlackboardKey.AddObjectFilter(this, GET_MEMBER_NAME_CHECKED(UDOGBTTaskBackAwayFromPlayer, BlackboardKey)); // for player blackborad input
	//BlackboardKey.AddFloatFilter(this, GET_MEMBER_NAME_CHECKED(UDOGBTTaskBackAwayFromPlayer, BlackboardKey)); // for range blackboard input
}

EBTNodeResult::Type UDOGBTTaskBackAwayFromPlayer::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	// Back away from player
	
	// Get Controller
	ADOGAIController* AIController = Cast<ADOGAIController>(OwnerComp.GetAIOwner());
	
	// Get the Pawn enemy base
	ADOGEnemyBase* AIPawn = AIController->GetPawn<ADOGEnemyBase>();

	// Location of the player
	const FVector TargetLocation = AIPawn->GetTarget()->GetTransform().GetLocation();

	// Location of the pawn
	const FVector PawnLocation = AIPawn->GetTransform().GetLocation();
	
	// Direction to move
	const FVector Dir = (TargetLocation - PawnLocation).GetSafeNormal() * Range;
	
	AIController->MoveToLocation(PawnLocation - Dir);
	
	FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
	return EBTNodeResult::Succeeded;
}

FString UDOGBTTaskBackAwayFromPlayer::GetStaticDescription() const
{
	return FString::Printf(TEXT("Distance To Move: %s"), *BlackboardKey.SelectedKeyName.ToString());
}


