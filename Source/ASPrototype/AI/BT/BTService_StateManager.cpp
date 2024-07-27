// Fill out your copyright notice in the Description page of Project Settings.

#include "AI/BT/BTService_StateManager.h"
#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Interface/ASEnemyInterface.h"
#include "Interface/GetSetBlackBoardDataInterface.h"

UBTService_StateManager::UBTService_StateManager()
{
	NodeName = TEXT("StateManager");
	Interval = 0.5f;
}

void UBTService_StateManager::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{	
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);
	APawn* ControllingPawn = OwnerComp.GetAIOwner()->GetPawn();
	if (ControllingPawn == nullptr)
	{
		return;
	}

	IASEnemyInterface* Enemy = Cast<IASEnemyInterface>(ControllingPawn);
	if (Enemy == nullptr)
	{
		return;
	}


	IGetSetBlackBoardDataInterface* BlackBoard = Cast<IGetSetBlackBoardDataInterface>(ControllingPawn->GetController());
	if (BlackBoard == nullptr)
	{
		return;
	}

	if (BlackBoard->GetBB_IsAlert())
	{
		state = 1;
	}
	else if (BlackBoard->GetBB_IsDetect())
	{
		state = 2;
	}
	else 
	{
		state = 0;
	}
	
	Enemy->OnChangeStateDelegate.Broadcast(state);
}


