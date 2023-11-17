// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "UObject/Object.h"
#include "DOGBTTaskAnimationDelay.generated.h"

/**
 * 
 */
UCLASS()
class MORRIS_API UDOGBTTaskAnimationDelay : public UBTTaskNode
{
	GENERATED_BODY()

public:
	UDOGBTTaskAnimationDelay();

	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
	virtual void TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;
	virtual FString GetStaticDescription() const override;
	
};
