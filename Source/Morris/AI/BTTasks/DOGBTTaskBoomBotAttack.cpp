// Fill out your copyright notice in the Description page of Project Settings.


#include "DOGBTTaskBoomBotAttack.h"

#include "Kismet/KismetSystemLibrary.h"
#include "Morris/AI/DOGAIController.h"
#include "Morris/AI/EnemyTypes/DOGEnemyBoomBot.h"
#include "Morris/Player/DOGPlayer.h"

UDOGBTTaskBoomBotAttack::UDOGBTTaskBoomBotAttack()
{
	NodeName = "Explosion Attack";
}

EBTNodeResult::Type UDOGBTTaskBoomBotAttack::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	ADOGAIController* AIController = Cast<ADOGAIController>(OwnerComp.GetAIOwner());
	ADOGEnemyBoomBot* AIPawn = AIController->GetPawn<ADOGEnemyBoomBot>();

	if (!IsValid(AIPawn))
	{
		FinishLatentTask(OwnerComp, EBTNodeResult::Failed);
		return EBTNodeResult::Failed;
	}
	
	// Ignore any specific actors
	const TArray<AActor*> IgnoreActors;
	
	// Array of actors that are inside the radius of the sphere
	TArray<AActor*> OutActors;

	TArray<TEnumAsByte<EObjectTypeQuery>> TraceObjectTypes;
	TraceObjectTypes.Add(UEngineTypes::ConvertToObjectType(ECollisionChannel::ECC_WorldDynamic));
	UClass* SeekPlayerClass = ADOGPlayer::StaticClass();

	// So Radius is not 0
	if (AIPawn->ExplosionRadius == 0)
	{
		AIPawn->ExplosionRadius = 200;
	}

	const FVector PawnLocation = AIPawn->GetTransform().GetLocation();
	
	// Checks if player is in the sphere
	if (UKismetSystemLibrary::SphereOverlapActors(GetWorld(), PawnLocation, AIPawn->ExplosionRadius, TraceObjectTypes, SeekPlayerClass, IgnoreActors, OutActors))
	{
		Cast<ADOGPlayer>(AIPawn->GetTarget())->ApplyDamage(AIPawn->Damage);
	}

	//DrawDebugSphere(GetWorld(), PawnLocation, AIPawn->ExplosionRadius, 12, FColor::Red, false, 0.2f);

	// Destroy Pawn
	AIPawn->Explode();
	
	FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
	return EBTNodeResult::Succeeded;
}

FString UDOGBTTaskBoomBotAttack::GetStaticDescription() const
{
	return FString::Printf(TEXT("Explode and kill itself - Takes Range and Damage from BP"));
	//: %s"), *BlackboardKey.SelectedKeyName.ToString(
}
