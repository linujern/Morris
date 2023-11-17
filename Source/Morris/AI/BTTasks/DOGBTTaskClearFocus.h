// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "UObject/Object.h"
#include "DOGBTTaskClearFocus.generated.h"

/**
 * 
 */
UCLASS()
class MORRIS_API UDOGBTTaskClearFocus : public UBTTaskNode
{
	GENERATED_BODY()
public:
	UDOGBTTaskClearFocus();

	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
	virtual FString GetStaticDescription() const override;
};

