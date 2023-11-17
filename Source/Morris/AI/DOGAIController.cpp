 // Fill out your copyright notice in the Description page of Project Settings.


#include "DOGAIController.h"
#include "DOGEnemyBase.h"
#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BlackboardComponent.h"


// Sets default values
ADOGAIController::ADOGAIController()
{
	
}

void ADOGAIController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);
	
	if (IsValid(BehaviorTreeAsset))
		RunBehaviorTree(BehaviorTreeAsset);
	
}





