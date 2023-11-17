// Fill out your copyright notice in the Description page of Project Settings.


#include "DOGBTDecoratorMoveToOrFromCheck.h"

#include "BehaviorTree/BlackboardComponent.h"
#include "Morris/AI/DOGAIController.h"
#include "Morris/AI/DOGEnemyBase.h"

UDOGBTDecoratorMoveToOrFromCheck::UDOGBTDecoratorMoveToOrFromCheck()
{
	NodeName = "Min Distance Check";

	BlackboardKey.AddFloatFilter(this, GET_MEMBER_NAME_CHECKED(UDOGBTDecoratorMoveToOrFromCheck, BlackboardKey));
}

bool UDOGBTDecoratorMoveToOrFromCheck::CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const
{
	ADOGAIController* AIController = Cast<ADOGAIController>(OwnerComp.GetAIOwner());
	ADOGEnemyBase* AIPawn = AIController->GetPawn<ADOGEnemyBase>();

	//const float DistanceToPlayer = AIController->GetBlackboardComponent()->GetValueAsFloat((BlackboardKey.SelectedKeyName));
	const float Dis = AIPawn->GetDistanceTo(AIPawn->GetTarget());

	float Distance = AIPawn->BackAwayRange;

	if (Distance == 0)
		Distance = MinDistance;
	
	
	if (IsValid(AIPawn) && Distance < Dis)
	{
		// Move To Player
		return true;
	}

	// Move From Player
	return false;
}

FString UDOGBTDecoratorMoveToOrFromCheck::GetStaticDescription() const
{
	return FString::Printf(TEXT("Returns true to if player is not in range: %s"), *BlackboardKey.SelectedKeyName.ToString());
}


