// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Tasks/BTTask_BlackboardBase.h"
#include "GameFramework/Actor.h"
#include "Morris/AI/DOGEnemyBase.h"
#include "DOGBTTaskMoveToPlayer.generated.h"

UCLASS()
class MORRIS_API UDOGBTTaskMoveToPlayer : public UBTTaskNode
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	UDOGBTTaskMoveToPlayer();

private:
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
	virtual void TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;
	virtual FString GetStaticDescription() const override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Chase", meta = (AllowPrivateAccess = true))
	TSubclassOf<APawn> Target;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Chase", meta = (AllowPrivateAccess = true))
	float Range = 100.0f;
};
