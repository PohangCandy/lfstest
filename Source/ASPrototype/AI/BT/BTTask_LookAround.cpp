// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/BT/BTTask_LookAround.h"
#include "AIController.h"
#include "Interface/ASEnemyInterface.h"

EBTNodeResult::Type UBTTask_LookAround::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	APawn* ControllingPawn = OwnerComp.GetAIOwner()->GetPawn();
	if (ControllingPawn == nullptr)
	{
		return EBTNodeResult::Failed;
	}

	IASEnemyInterface* EnemyIntarface = Cast<IASEnemyInterface>(ControllingPawn);
	if (EnemyIntarface == nullptr)
	{
		return EBTNodeResult::Failed;
	}

	EnemyIntarface->InitState();

	return EBTNodeResult::Succeeded;
	//식별 범위 2배로 키우고 좌우로 회전하는 애니메이션 적용.  추후에 의심상태에서 플레이어 식별시 바로 발각상태로 변하도록 로직 변경. 
}
