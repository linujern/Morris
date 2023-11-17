// Fill out your copyright notice in the Description page of Project Settings.


#include "DOGBTTaskAnimationDelay.h"

#include "Morris/AI/DOGAIController.h"
#include "Morris/AI/DOGEnemyBase.h"

UDOGBTTaskAnimationDelay::UDOGBTTaskAnimationDelay()
{
	NodeName = "Animation Delay";
	bTickIntervals = true;
	INIT_TASK_NODE_NOTIFY_FLAGS();
}

EBTNodeResult::Type UDOGBTTaskAnimationDelay::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{

	const ADOGAIController* AIController = Cast<ADOGAIController>(OwnerComp.GetAIOwner());
	const ADOGEnemyBase* AIPawn = AIController->GetPawn<ADOGEnemyBase>();

	float DelayTime = AIPawn->AttackAnimationDelay;

	if (DelayTime == 0)
		DelayTime = 3.f;
	
	SetNextTickTime(NodeMemory, DelayTime);
	
	return EBTNodeResult::InProgress;
}

void UDOGBTTaskAnimationDelay::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	// Using the SetNextTickTime in ExecuteTask we are certain we are only getting ticked when the wait is finished
	ensure(GetSpecialNodeMemory<FBTTaskMemory>(NodeMemory)->NextTickRemainingTime <= 0.f);

	// continue execution from this node
	FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
}

FString UDOGBTTaskAnimationDelay::GetStaticDescription() const
{
	return FString::Printf(TEXT("Takes Delay from Blueprint"));
}