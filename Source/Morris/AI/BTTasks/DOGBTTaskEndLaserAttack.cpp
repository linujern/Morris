// Fill out your copyright notice in the Description page of Project Settings.


#include "DOGBTTaskEndLaserAttack.h"

#include "Morris/AI/DOGAIController.h"
#include "Morris/AI/EnemyTypes/DOGEnemyLaserBot.h"

UDOGBTTaskEndLaserAttack::UDOGBTTaskEndLaserAttack()
{
	NodeName = "Stop Laser Attack";
}

EBTNodeResult::Type UDOGBTTaskEndLaserAttack::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	ADOGAIController* AIController = Cast<ADOGAIController>(OwnerComp.GetAIOwner());
	ADOGEnemyLaserBot* AIPawn = AIController->GetPawn<ADOGEnemyLaserBot>();

	// Stop looking around and stay still
	//AIController->StopMovement();
	//AIController->ClearFocus(EAIFocusPriority::Gameplay);
	
	AIPawn->StopBeam();

	FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
	return EBTNodeResult::Succeeded;
}

FString UDOGBTTaskEndLaserAttack::GetStaticDescription() const
{
	return FString::Printf(TEXT("Only for LaserBot"));
}
