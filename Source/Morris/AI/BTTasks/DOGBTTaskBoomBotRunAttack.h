// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "Morris/AI/DOGAIController.h"
#include "UObject/Object.h"
#include "DOGBTTaskBoomBotRunAttack.generated.h"

/**
 * 
 */
UCLASS()
class MORRIS_API UDOGBTTaskBoomBotRunAttack : public UBTTaskNode
{
	GENERATED_BODY()
public:
	UDOGBTTaskBoomBotRunAttack();

	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
	virtual void TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;
	virtual FString GetStaticDescription() const override;

	UFUNCTION()
	void StopCheck();
	
	UPROPERTY()
	FTimerHandle StopCheckHandle;

	UPROPERTY()
	ADOGAIController* AIController2;
};
