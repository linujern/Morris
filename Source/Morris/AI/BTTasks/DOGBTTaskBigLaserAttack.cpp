// Fill out your copyright notice in the Description page of Project Settings.


#include "DOGBTTaskBigLaserAttack.h"

#include "Morris/AI/DOGAIController.h"
#include "Morris/AI/EnemyTypes/DOGEnemyLaserBot.h"

UDOGBTTaskBigLaserAttack::UDOGBTTaskBigLaserAttack()
{
	NodeName = "Big Laser Attack";
}

EBTNodeResult::Type UDOGBTTaskBigLaserAttack::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	ADOGAIController* AIController = Cast<ADOGAIController>(OwnerComp.GetAIOwner());
	ADOGEnemyLaserBot* AIPawn = AIController->GetPawn<ADOGEnemyLaserBot>();

	// Stop looking around and stay still
	AIController->StopMovement();
	AIController->ClearFocus(EAIFocusPriority::Gameplay);
	
	AIPawn->StartBeam();

	FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
	return EBTNodeResult::Succeeded;
}

FString UDOGBTTaskBigLaserAttack::GetStaticDescription() const
{
	return FString::Printf(TEXT("Only for LaserBot"));
}
