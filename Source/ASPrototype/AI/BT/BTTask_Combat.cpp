// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/BT/BTTask_Combat.h"
#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Interface/ASEnemyInterface.h"

UBTTask_Combat::UBTTask_Combat()
{
	NodeName = TEXT("Shooting");
	bNotifyTick = true;
	IsPlaying =false;
}

EBTNodeResult::Type UBTTask_Combat::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{	
	APawn* ControllingPawn = OwnerComp.GetAIOwner()->GetPawn();
	if (ControllingPawn == nullptr)
	{
		return EBTNodeResult::Failed;
	}

	IASEnemyInterface* Enemy = Cast<IASEnemyInterface>(ControllingPawn);
	if (Enemy==nullptr)
	{
		return EBTNodeResult::Failed;
	}

	//Enemy->PlayAttackAnimation();
	//Enemy->CurState = EState::Attack;
	
	//IsPlaying = true;
	//Enemy->OnAttackEnd.AddLambda([this]()->void {IsPlaying = false; });
	//ANIM->
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
