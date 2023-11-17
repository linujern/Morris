// Fill out your copyright notice in the Description page of Project Settings.


#include "DOGBTTaskRangedEnemyAttack.h"

#include "BehaviorTree/BlackboardComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Morris/AI/DOGAIController.h"
#include "Morris/AI/EnemyTypes/DOGEnemyRanged.h"
#include "Morris/AI/EnemyTypes/DOGEnemyRangedBullet.h"


UDOGBTTaskRangedEnemyAttack::UDOGBTTaskRangedEnemyAttack()
{
	NodeName = "Ranged Enemy Attack";
	bNotifyTick = true;
	//BlackboardKey.AddClassFilter(this, GET_MEMBER_NAME_CHECKED(UDOGBTTaskRangedEnemyAttack, BlackboardKey), );
	//BlackboardKey.AddObjectFilter(this, GET_MEMBER_NAME_CHECKED(UDOGBTTaskRangedEnemyAttack, BlackboardKey), );

}

void UDOGBTTaskRangedEnemyAttack::Cooldown()
{
	bWait = false;
}


EBTNodeResult::Type UDOGBTTaskRangedEnemyAttack::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	// Get Controller
	ADOGAIController* AIController = Cast<ADOGAIController>(OwnerComp.GetAIOwner());
	
	// Get the Pawn enemy base
	ADOGEnemyRanged* AIPawn = AIController->GetPawn<ADOGEnemyRanged>();
	
	AActor* Player = AIPawn->GetTarget();

	ShotsFired = 0;
	
	if (!IsValid(AIController) && !IsValid(AIPawn))
	{
		UE_LOG(LogTemp, Warning, TEXT("Ranged enemy attack task - AIController or AIPawn Invaild"))
		return EBTNodeResult::Aborted;
	}
	
	if (AIController->GetFocusActor() != Player)
		AIController->SetFocus(Player, EAIFocusPriority::Gameplay);
	
	return  EBTNodeResult::InProgress;
}

void UDOGBTTaskRangedEnemyAttack::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	if (bWait) return;
	
	// Get Controller
	ADOGAIController* AIController = Cast<ADOGAIController>(OwnerComp.GetAIOwner());
	
	// Get the Pawn enemy base
	ADOGEnemyRanged* AIPawn = AIController->GetPawn<ADOGEnemyRanged>();
	
	AIPawn->FireBullet(AIController->GetBlackboardComponent()->GetValueAsClass(BlackboardKey.SelectedKeyName));
	ShotsFired++;
	
	if (ShotsFired >= 3)
    {
		FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
    }
	bWait = true;
	FTimerHandle Handle;
	GetWorld()->GetTimerManager().SetTimer(Handle, this, &UDOGBTTaskRangedEnemyAttack::Cooldown, 0.7f, false);

	//FinishLatentTask(OwnerComp, EBTNodeResult::InProgress);
}


FString UDOGBTTaskRangedEnemyAttack::GetStaticDescription() const
{
	return FString::Printf(TEXT("Bullet BP Class: %s"), *BlackboardKey.SelectedKeyName.ToString());
}

