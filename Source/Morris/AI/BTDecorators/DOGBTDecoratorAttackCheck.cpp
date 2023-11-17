// Fill out your copyright notice in the Description page of Project Settings.


#include "DOGBTDecoratorAttackCheck.h"
#include "Morris/AI/DOGAIController.h"
#include "Morris/AI/DOGEnemyBase.h"

UDOGBTDecoratorAttackCheck::UDOGBTDecoratorAttackCheck()
{
	NodeName = "Check If Attack Conditions are Ready";
}

bool UDOGBTDecoratorAttackCheck::CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const
{
	Super::CalculateRawConditionValue(OwnerComp, NodeMemory);

	ADOGAIController* AIController = Cast<ADOGAIController>(OwnerComp.GetAIOwner());
	ADOGEnemyBase* AIPawn = AIController->GetPawn<ADOGEnemyBase>();
	
	const float Dis = AIPawn->GetDistanceTo(AIPawn->GetTarget());

	float CheckRange = AIPawn->ActivateAttackRange;

	if (CheckRange == 0)
		CheckRange = Range;
	
	
	if (IsValid(AIPawn) && CheckRange > Dis && AIController->LineOfSightTo(AIPawn->GetTarget()))
	{
		// In Range and vision
		return true;
	}
	// Out of range or no vision
	return false;
}

FString UDOGBTDecoratorAttackCheck::GetStaticDescription() const
{
	return FString::Printf(TEXT("Returns true if in Range and In Sight"));
}
