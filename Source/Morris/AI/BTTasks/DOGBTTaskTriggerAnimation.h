// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Tasks/BTTask_BlackboardBase.h"
#include "UObject/Object.h"
#include "DOGBTTaskTriggerAnimation.generated.h"

/**
 * 
 */
UCLASS()
class MORRIS_API UDOGBTTaskTriggerAnimation : public UBTTask_BlackboardBase
{
	GENERATED_BODY()
public:
	UDOGBTTaskTriggerAnimation();
private:
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
	virtual FString GetStaticDescription() const override;
};
