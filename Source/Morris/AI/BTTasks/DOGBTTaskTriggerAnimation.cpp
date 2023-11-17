// Fill out your copyright notice in the Description page of Project Settings.


#include "DOGBTTaskTriggerAnimation.h"

#include "Morris/AI/DOGAIController.h"
#include "Morris/AI/DOGEnemyBase.h"

UDOGBTTaskTriggerAnimation::UDOGBTTaskTriggerAnimation()
{
	NodeName = "Trigger Animation";
}

EBTNodeResult::Type UDOGBTTaskTriggerAnimation::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	ADOGAIController* AIController = Cast<ADOGAIController>(OwnerComp.GetAIOwner());
	ADOGEnemyBase* AIPawn = AIController->GetPawn<ADOGEnemyBase>();

	AIController->SetFocus(AIPawn->GetTarget());
	
	// Trigger a blueprintable event
	AIPawn->OnTriggerAnimation();
	
	FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
	return  EBTNodeResult::Succeeded;
}

FString UDOGBTTaskTriggerAnimation::GetStaticDescription() const
{
	return FString::Printf(TEXT("Triggers OnTriggerAnimation a blueprintable event"));
}
