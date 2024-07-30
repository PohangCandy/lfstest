// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/BT/BTTask_Combat.h"
#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Interface/ASEnemyInterface.h"

UBTTask_Combat::UBTTask_Combat()
{
	NodeName = TEXT("Shooting");
}

EBTNodeResult::Type UBTTask_Combat::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{	
	APawn* ControllingPawn = OwnerComp.GetAIOwner()->GetPawn();
	if (ControllingPawn == nullptr)
	{
		return EBTNodeResult::Failed;
	}

	IASEnemyInterface* EnemyInterface = Cast<IASEnemyInterface>(ControllingPawn);
	if (EnemyInterface ==nullptr)
	{
		return EBTNodeResult::Failed;
	}
	FOnAttackEndDelegate AttackEndDelegate;
	AttackEndDelegate.BindLambda(
		[&]()
		{
			FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded); 
		}
	);
	EnemyInterface->SetAIAttackDelegate(AttackEndDelegate);
	EnemyInterface->Attack();
	return EBTNodeResult::Succeeded;
}

//
//void UBTTask_Combat::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
//{
//
//	Super::TickTask(OwnerComp, NodeMemory, DeltaSeconds);
//	APawn* ControllingPawn = OwnerComp.GetAIOwner()->GetPawn();
//	Enemy = Cast<AASEnemyCharacter>(ControllingPawn);
//	AI = Cast<AASAIController>(Enemy->GetController());
//	
//	if (!(IsPlaying))
//	{	
//		FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
//	}
//}
