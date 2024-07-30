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
	//�ĺ� ���� 2��� Ű��� �¿�� ȸ���ϴ� �ִϸ��̼� ����.  ���Ŀ� �ǽɻ��¿��� �÷��̾� �ĺ��� �ٷ� �߰����·� ���ϵ��� ���� ����. 
}
