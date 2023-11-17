// Fill out your copyright notice in the Description page of Project Settings.


#include "DOGBTTaskSetFocus.h"

#include "Morris/AI/DOGAIController.h"
#include "Morris/AI/DOGEnemyBase.h"

UDOGBTTaskSetFocus::UDOGBTTaskSetFocus()
{
	NodeName = "Set Focus";
}

EBTNodeResult::Type UDOGBTTaskSetFocus::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	ADOGAIController* AIController = Cast<ADOGAIController>(OwnerComp.GetAIOwner()); 
	AIController->SetFocus(AIController->GetPawn<ADOGEnemyBase>()->GetTarget(), EAIFocusPriority::Gameplay);
	
	FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
	return EBTNodeResult::Succeeded;
}

FString UDOGBTTaskSetFocus::GetStaticDescription() const
{
	return FString::Printf(TEXT("Set Focus on Player"));
}
