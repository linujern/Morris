// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "UObject/Object.h"
#include "DOGBTTaskSetFocus.generated.h"

/**
 * 
 */
UCLASS()
class MORRIS_API UDOGBTTaskSetFocus : public UBTTaskNode
{
	GENERATED_BODY()
public:
	UDOGBTTaskSetFocus();

	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
	virtual FString GetStaticDescription() const override;
};
