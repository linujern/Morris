// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Tasks/BTTask_BlackboardBase.h"
#include "DOGBTTaskBackAwayFromPlayer.generated.h"

/**
 * 
 */
UCLASS()
class MORRIS_API UDOGBTTaskBackAwayFromPlayer : public UBTTask_BlackboardBase
{
	GENERATED_BODY()
public:
	UDOGBTTaskBackAwayFromPlayer();
	
private:
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
	virtual FString GetStaticDescription() const override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Back Away Stats", meta = (AllowPrivateAccess))
	float Range = 400.0f;
};
