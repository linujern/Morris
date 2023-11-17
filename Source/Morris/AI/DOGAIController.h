// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
//#include "DOGEnemyBase.h"
#include "BehaviorTree/BehaviorTreeComponent.h"
#include "DOGAIController.generated.h"

class ADOGEnemyBase;

UCLASS()
class MORRIS_API ADOGAIController : public AAIController
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	ADOGAIController();

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	TObjectPtr<UBehaviorTree> BehaviorTreeAsset;

protected:
	// Called when the game starts or when spawned
	//virtual void BeginPlay() override;
    
	virtual void OnPossess(APawn* InPawn) override;

    
private:

	

	
};
