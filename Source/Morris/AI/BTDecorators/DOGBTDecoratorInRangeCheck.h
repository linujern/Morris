// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Decorators/BTDecorator_BlackboardBase.h"
#include "UObject/Object.h"
#include "DOGBTDecoratorInRangeCheck.generated.h"

/**
 * 
 */
UCLASS()
class MORRIS_API UDOGBTDecoratorInRangeCheck : public UBTDecorator_BlackboardBase
{
	GENERATED_BODY()

public:
	UDOGBTDecoratorInRangeCheck();
private:
	virtual bool CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const override;
	virtual FString GetStaticDescription() const override;

	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Stats", meta = (AllowPrivateAccess))
	float Range = 100.0f;

};
