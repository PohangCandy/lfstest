// Fill out your copyright notice in the Description page of Project Settings.

#include "AI/ASAIController.h"
#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BlackboardData.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "ASAI.h"

AASAIController::AASAIController()
{

	//BB,BT 
	static ConstructorHelpers::FObjectFinder<UBlackboardData> BBAssetRef(TEXT("/Game/ASPrototype/Enemy/AI/BB_Enemy.BB_Enemy"));
	if (nullptr != BBAssetRef.Object)
	{
		BBAsset = BBAssetRef.Object;
	}
	
	static ConstructorHelpers::FObjectFinder<UBehaviorTree> BTAssetRef(TEXT("/Game/ASPrototype/Enemy/AI/BT_Enemy.BT_Enemy"));
	if (nullptr != BTAssetRef.Object)
	{
		BTAsset = BTAssetRef.Object;
	}

	static ConstructorHelpers::FObjectFinder<UBehaviorTree> BT_FinalTargetAssetRef(TEXT("/Script/AIModule.BehaviorTree'/Game/ASPrototype/Enemy/AI/BT_FinalTarget.BT_FinalTarget'"));
	ensure(BT_FinalTargetAssetRef.Object);
	if (nullptr != BT_FinalTargetAssetRef.Object)
	{
		BT_FinalTargetAsset = BT_FinalTargetAssetRef.Object;
	}

}


//Init 
void AASAIController::BeginPlay()
{
	Super::BeginPlay();
}
void AASAIController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn); //빙의 시작  , 에너미의 소유권은 AIController가 얻게 됨
	RunAI();
}
void AASAIController::RunAI()
{
	UBlackboardComponent* BlackboardPtr = Blackboard.Get();
	if (!UseBlackboard(BBAsset, BlackboardPtr)) { return; }
	RunBehaviorTree(BTAsset);
}
void AASAIController::StopAI()
{
	UBehaviorTreeComponent* BTComponent = Cast<UBehaviorTreeComponent>(BrainComponent);
	if (BTComponent)
	{
		BTComponent->StopTree();
	}
}


void AASAIController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}



//위치 관련
//void AASAIController::FindNearLocation(FVector Loc, float Range)
//{
//	//LastKnownPosition 기준으로 랜덤 위치 얻기 
//	UNavigationSystemV1* NavSystem = UNavigationSystemV1::GetNavigationSystem(EnemyRef->GetWorld());
//	FNavLocation NextPosition;
//	if (NavSystem->GetRandomPointInNavigableRadius(Loc, Range, NextPosition))
//	{
//		SetBB_LastKnownPosition(NextPosition.Location);
//	}
//}


//BB 데이터 접근 
void AASAIController::SetBB_Target(UObject* actor)
{
	GetBlackboardComponent()->SetValueAsObject(BBKEY_TARGET, actor);
}
UObject* AASAIController::GetBB_Target()
{
	return GetBlackboardComponent()->GetValueAsObject(BBKEY_TARGET);
}

void AASAIController::SetBB_IsDetect(bool b)
{	
	GetBlackboardComponent()->SetValueAsBool(BBKEY_IsDetect, b);
	
}
bool AASAIController::GetBB_IsDetect()
{	
	return GetBlackboardComponent()->GetValueAsBool(BBKEY_IsDetect);
}

void AASAIController::SetBB_IsAlert(bool b)
{
	GetBlackboardComponent()->SetValueAsBool(BBKEY_IsAlert,b);
}
bool AASAIController::GetBB_IsAlert()
{
	return GetBlackboardComponent()->GetValueAsBool(BBKEY_IsAlert);
}

void AASAIController::SetBB_PatrolLoc(FVector vector)
{
	GetBlackboardComponent()->SetValueAsVector(BBKEY_PATROLLOC, vector);
}
FVector AASAIController::GetBB_PatrolLoc()
{
	return GetBlackboardComponent()->GetValueAsVector(BBKEY_PATROLLOC);
}

void AASAIController::SetBB_PathLoc(FVector vector)
{
	GetBlackboardComponent()->SetValueAsVector(BBKEY_PathLOC, vector);
}
FVector AASAIController::GetBB_PathLoc()
{
	return GetBlackboardComponent()->GetValueAsVector(BBKEY_PathLOC);
}

void AASAIController::SetBB_AttackRange(FVector vector)
{
	GetBlackboardComponent()->SetValueAsVector(BBKEY_AttackRange, vector);
}
FVector AASAIController::GetBB_AttackRange()
{
	return GetBlackboardComponent()->GetValueAsVector(BBKEY_AttackRange);
}

void AASAIController::SetBB_CanShootTarget(bool b)
{
	GetBlackboardComponent()->SetValueAsBool(BBKEY_CanShootTarget, b);
}
bool AASAIController::GetBB_CanShootTarget()
{
	return GetBlackboardComponent()->GetValueAsBool(BBKEY_CanShootTarget);
}

void AASAIController::SetBB_LastKnownPosition(FVector vector)
{
	GetBlackboardComponent()->SetValueAsVector(BBKEY_LastKnownPosition,vector);
}
FVector AASAIController::GetBB_LastKnownPosition()
{
	return GetBlackboardComponent()->GetValueAsVector(BBKEY_LastKnownPosition);
}








//생성자에서만 Casting 안됨 
//TSubclassOf<UUserWidget> WidgetObject = DetectBar->GetWidgetClass();
//UASDetectWidget* uiRef = Cast<UASDetectWidget>(WidgetObject);
//ensure(UiRef);
//UiRef->SetOwner(this);

//ID 세팅 
//AASEnemyBase* EnemyRef = Cast<AASEnemyBase>(GetPawn());
//if (EnemyRef)
//{
//	TeamId = FGenericTeamId(EnemyRef->ID);
//}


//ETeamAttitude::Type AASAIController::GetTeamAttitudeTowards(const AActor& Other) const
//{
//	return ETeamAttitude::Type();
//}
//GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Blue, FString::Printf(TEXT("DetectionLevel : %f"), DetectionLevel));
//적과 플레이어 사이 거리 얻기
//FVector Location_Between_Player_And_Enemy = EnemyLoc - PlayerLoc;
//DistanceDifference_Value = Location_Between_Player_And_Enemy.Length();
//void AASAIController::On_Updated(TArray<AActor*> const& updated_Actors)
//{
//	
//	for (size_t i = 0; i < updated_Actors.Num(); i++)
//	{
//		GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Green, FString::Printf(TEXT("On_Updated , EventOnBySound is %d"), EventOnBySound));
//		FActorPerceptionBlueprintInfo info;
//		AIPerComp->GetActorsPerception(updated_Actors[i], info);
//		for (size_t k = 0; k < info.LastSensedStimuli.Num(); k++)
//		{
//			FAIStimulus const stim = info.LastSensedStimuli[k];
//			if (stim.Tag==tags::lvl1_Sound_tag) //작은 소리는 의심상태 
//			{
//				EventOnBySound = true;
//				SetBB_IsAlert(true);
//				SetBB_LastKnownPosition(stim.StimulusLocation);
//			}
//			else if (stim.Tag == tags::lvl2_Sound_tag) //큰 소리는 공격상태 
//			{
//				EventOnBySound = true;
//				SetBB_IsDetect(true);
//			}
//			else
//			{
//				EventOnBySound = false;
//				AASCharacterPlayer* CurPlayer = Cast<AASCharacterPlayer>(updated_Actors[i]);
//				CheckPlayer(CurPlayer);
//			}
//
//		}
//	}
//}
//void AASAIController::OnPawnDetected(const TArray<AActor*>& DetectedPawns) //적 둘이 마주보면 문제 발생 
//{	
//	for (int32 i = 0; i < DetectedPawns.Num(); ++i)
//	{
//		AASCharacterPlayer* CurPlayer = Cast<AASCharacterPlayer>(DetectedPawns[i]);
//		//ensure(CurPlayer);
//		CheckPlayer(CurPlayer);
//	}
//}