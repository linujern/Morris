// Fill out your copyright notice in the Description page of Project Settings.


#include "DOGBTDecoratorInRangeCheck.h"

#include "Morris/AI/DOGAIController.h"
#include "Morris/AI/DOGEnemyBase.h"

UDOGBTDecoratorInRangeCheck::UDOGBTDecoratorInRangeCheck()
{
	NodeName = "In Range Check";
}

bool UDOGBTDecoratorInRangeCheck::CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const
{
	Super::CalculateRawConditionValue(OwnerComp, NodeMemory);
	ADOGAIController* AIController = Cast<ADOGAIController>(OwnerComp.GetAIOwner());
	ADOGEnemyBase* AIPawn = AIController->GetPawn<ADOGEnemyBase>();

	//const float DistanceToPlayer = AIController->GetBlackboardComponent()->GetValueAsFloat((BlackboardKey.SelectedKeyName));
	const float Dis = AIPawn->GetDistanceTo(AIPawn->GetTarget());

	float InRange = AIPawn->InRange;

	if (InRange == 0)
		InRange = Range;
	
	if (IsValid(AIPawn) && InRange > Dis)
	{
		// In Range
		return true;
	}
	// Out of range
	return false;
}

FString UDOGBTDecoratorInRangeCheck::GetStaticDescription() const
{
	return FString::Printf(TEXT("Returns true if in distance"));
}
