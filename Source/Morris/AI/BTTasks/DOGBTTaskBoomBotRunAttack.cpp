// Fill out your copyright notice in the Description page of Project Settings.


#include "DOGBTTaskBoomBotRunAttack.h"

#include "Morris/AI/DOGAIController.h"
#include "Morris/AI/EnemyTypes/DOGEnemyBoomBot.h"

UDOGBTTaskBoomBotRunAttack::UDOGBTTaskBoomBotRunAttack()
{
	NodeName = "Run Attack";
	bTickIntervals = true;
	INIT_TASK_NODE_NOTIFY_FLAGS();

}

EBTNodeResult::Type UDOGBTTaskBoomBotRunAttack::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	ADOGAIController* AIController = Cast<ADOGAIController>(OwnerComp.GetAIOwner());
	ADOGEnemyBoomBot* AIPawn = AIController->GetPawn<ADOGEnemyBoomBot>();
	AIController2 = AIController;

	float DelayTime = AIPawn->AttackAnimationDelay;

	if (DelayTime == 0)
		DelayTime = 2.f;
	
	AIController->MoveToActor(AIPawn->GetTarget());
	

	SetNextTickTime(NodeMemory, DelayTime);

	GetWorld()->GetTimerManager().SetTimer(StopCheckHandle, this, &UDOGBTTaskBoomBotRunAttack::StopCheck, 0.05f, true);
	
	return EBTNodeResult::InProgress;
	
}

void UDOGBTTaskBoomBotRunAttack::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	ADOGAIController* AIController = Cast<ADOGAIController>(OwnerComp.GetAIOwner());
	ADOGEnemyBoomBot* AIPawn = AIController->GetPawn<ADOGEnemyBoomBot>();
	AIController->MoveToActor(AIPawn->GetTarget());
	
	// Using the SetNextTickTime in ExecuteTask we are certain we are only getting ticked when the wait is finished
	ensure(GetSpecialNodeMemory<FBTTaskMemory>(NodeMemory)->NextTickRemainingTime <= 0.f);

	// continue execution from this node
	FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
}


FString UDOGBTTaskBoomBotRunAttack::GetStaticDescription() const
{
	return FString::Printf(TEXT("What do you think it does"));
}

void UDOGBTTaskBoomBotRunAttack::StopCheck()
{
	if (IsValid(AIController2))
		AIController2->MoveToActor(AIController2->GetPawn<ADOGEnemyBase>()->GetTarget());
}
