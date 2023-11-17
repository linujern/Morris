// Fill out your copyright notice in the Description page of Project Settings.


#include "DOGBTTaskClearFocus.h"

#include "Morris/AI/DOGAIController.h"

UDOGBTTaskClearFocus::UDOGBTTaskClearFocus()
{
	NodeName = "Clear Focus";
}

EBTNodeResult::Type UDOGBTTaskClearFocus::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	Cast<ADOGAIController>(OwnerComp.GetAIOwner())->ClearFocus(EAIFocusPriority::Gameplay);
	
	FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
	return EBTNodeResult::Succeeded;
}

FString UDOGBTTaskClearFocus::GetStaticDescription() const
{
	return FString::Printf(TEXT("What do you think it does"));

}
