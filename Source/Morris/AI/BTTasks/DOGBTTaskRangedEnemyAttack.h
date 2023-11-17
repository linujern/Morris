// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Tasks/BTTask_BlackboardBase.h"
#include "Morris/AI/DOGAIController.h"
#include "Morris/AI/EnemyTypes/DOGEnemyRanged.h"
#include "UObject/Object.h"
#include "DOGBTTaskRangedEnemyAttack.generated.h"

/**
 * 
 */
UCLASS()
class MORRIS_API UDOGBTTaskRangedEnemyAttack : public UBTTask_BlackboardBase
{
	GENERATED_BODY()
public:
	UDOGBTTaskRangedEnemyAttack();

	UFUNCTION()
	void Cooldown();
	
private:
	
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
	virtual void TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;
	virtual FString GetStaticDescription() const override;
private:
	UPROPERTY()
	int ShotsFired = 0.f;

	UPROPERTY()
	bool bWait = false;
};
