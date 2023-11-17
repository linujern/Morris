// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BehaviorTree/Tasks/BTTask_BlackboardBase.h"
#include "Navigation/PathFollowingComponent.h"
#include "DOGBTTaskStrafe.generated.h"

/**
 * 
 */
UCLASS()
class MORRIS_API UDOGBTTaskStrafe : public UBTTask_BlackboardBase
{
	GENERATED_BODY()

public:
	UDOGBTTaskStrafe();

	void BugBool() { AntiBugBool = true; Handle.Invalidate(); }
protected:
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
	virtual void TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;
	virtual FString GetStaticDescription() const override;

private:
	UPROPERTY()
	FVector Location;

	UPROPERTY()
	FTimerHandle Handle;

	UPROPERTY()
	bool AntiBugBool = false;
};
