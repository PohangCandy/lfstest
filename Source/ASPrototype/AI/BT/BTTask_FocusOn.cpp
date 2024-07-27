// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/BT/BTTask_FocusOn.h"
#include "AIController.h"
#include "Kismet/KismetMathLibrary.h"
#include "Math/UnrealMathUtility.h"
#include "Interface/ASEnemyInterface.h"
#include "Interface/GetSetBlackBoardDataInterface.h"

EBTNodeResult::Type UBTTask_FocusOn::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	APawn* ControllingPawn = OwnerComp.GetAIOwner()->GetPawn();
	if (ControllingPawn == nullptr)
	{
		return EBTNodeResult::Failed;
	}

	IASEnemyInterface* Enemy = Cast<IASEnemyInterface>(ControllingPawn);
	if (Enemy == nullptr)
	{
		return EBTNodeResult::Failed;
	}

	IGetSetBlackBoardDataInterface* BlackBoard = Cast<IGetSetBlackBoardDataInterface>(ControllingPawn->GetController());
	if (BlackBoard == nullptr)
	{
		return EBTNodeResult::Failed;
	}

	Enemy->TurnToTarget(BlackBoard->GetBB_LastKnownPosition());

	return EBTNodeResult::Succeeded;
}
