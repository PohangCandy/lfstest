// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include "CoreMinimal.h"
#include "AIController.h"
#include "Interface/GetSetBlackBoardDataInterface.h"
#include "ASAIController.generated.h"


UCLASS()
class ASPROTOTYPE_API AASAIController : public AAIController,public IGetSetBlackBoardDataInterface
{
	GENERATED_BODY()

public:
	AASAIController();

	//Init
	virtual void Tick(float DeltaTime);
	virtual void BeginPlay() override;
	virtual void OnPossess(APawn* InPawn) override;
	void RunAI();
	void StopAI();





	//BB 데이터 정보 
	virtual void SetBB_LastKnownPosition(FVector vector) override;
	virtual FVector GetBB_LastKnownPosition() override;

	virtual void SetBB_Target(UObject* object) override;
	virtual UObject* GetBB_Target() override;

	virtual void SetBB_IsDetect(bool b) override;
	virtual bool GetBB_IsDetect() override;

	virtual void SetBB_IsAlert(bool b) override;
	virtual bool GetBB_IsAlert() override;

	virtual void SetBB_CanShootTarget(bool b) override;
	virtual bool GetBB_CanShootTarget() override;

	virtual void SetBB_PatrolLoc(FVector vector) override;
	virtual FVector GetBB_PatrolLoc() override;

	virtual void SetBB_PathLoc(FVector vector) override;
	virtual FVector GetBB_PathLoc() override;

	virtual void SetBB_AttackRange(FVector vector) override;
	virtual FVector GetBB_AttackRange() override;




private:
	//BB,BT 
	UPROPERTY()
	TObjectPtr<class UBlackboardData> BBAsset;
	UPROPERTY()
	TObjectPtr<class UBehaviorTree> BTAsset;
	UPROPERTY()
	TObjectPtr<class UBehaviorTree> BT_FinalTargetAsset;
	UPROPERTY()
	TObjectPtr<class UBehaviorTree> BT_SniperAsset;
	UPROPERTY()
	TObjectPtr<class UBehaviorTree> BT_HeavyAsset;


};
