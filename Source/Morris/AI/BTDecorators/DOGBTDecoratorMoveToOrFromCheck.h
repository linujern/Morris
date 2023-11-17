// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Decorators/BTDecorator_BlackboardBase.h"
#include "UObject/Object.h"
#include "DOGBTDecoratorMoveToOrFromCheck.generated.h"

/**
 * 
 */
UCLASS()
class MORRIS_API UDOGBTDecoratorMoveToOrFromCheck : public UBTDecorator_BlackboardBase
{
	GENERATED_BODY()
public:
	UDOGBTDecoratorMoveToOrFromCheck();
	
private:
	virtual bool CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const override;
	virtual FString GetStaticDescription() const override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Back Away Stats", meta = (AllowPrivateAccess))
	float MinDistance = 100.0f;
	
};
