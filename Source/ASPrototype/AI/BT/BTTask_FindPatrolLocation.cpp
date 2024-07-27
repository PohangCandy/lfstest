// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/BT/BTTask_FindPatrolLocation.h"
#include "AIController.h"
#include "Enemy/PatrolPath.h"
#include "Interface/GetSetBlackBoardDataInterface.h"
#include "Interface/ASEnemyInterface.h"


UBTTask_FindPatrolLocation::UBTTask_FindPatrolLocation()
{
	NodeName = TEXT("FindPatrolLocation");
}

EBTNodeResult::Type UBTTask_FindPatrolLocation::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{	
	APawn* ControllingPawn = OwnerComp.GetAIOwner()->GetPawn();
	if (ControllingPawn == nullptr)
	{
		return EBTNodeResult::Failed;
	}
	
	AAIController* AI = Cast<AAIController>(ControllingPawn->GetController());
	IASEnemyInterface* Enemy = Cast<IASEnemyInterface>(ControllingPawn);
	if (Enemy == nullptr || AI == nullptr)
	{
		return EBTNodeResult::Failed;
	}
	
	APatrolPath* PatrolpathActor = Cast<APatrolPath>(Enemy->GetPatrolPath());
	if (PatrolpathActor == nullptr) { return EBTNodeResult::Failed; }
	IGetSetBlackBoardDataInterface* BBData = Cast<IGetSetBlackBoardDataInterface>(ControllingPawn->GetController());
	int length;
	TArray<FVector> PArray =  PatrolpathActor->PatrolPoints;
	
	length = PArray.Num()-1;
	if (PatrolpathActor->idx <= length)
	{	
		BBData->SetBB_PathLoc(PArray[PatrolpathActor->idx]);
		PatrolpathActor->idx +=1;
	}
	else
	{
		PatrolpathActor->idx = 0;
	}
	//출력 인덱스 테스트
	GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Red, FString::Printf(TEXT(" curIdx : %d "), PatrolpathActor->idx));
	return EBTNodeResult::Succeeded;
}