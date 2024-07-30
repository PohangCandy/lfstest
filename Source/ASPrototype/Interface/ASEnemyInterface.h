// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "ASEnemyInterface.generated.h"

DECLARE_MULTICAST_DELEGATE_OneParam(FOnChangeStateDelegate, uint8 /*state*/);
DECLARE_DELEGATE(FOnAttackEndDelegate)

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UASEnemyInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class ASPROTOTYPE_API IASEnemyInterface
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	virtual AActor* GetPatrolPath()=0;
	virtual FVector GetTargetLocation() = 0;
	virtual AActor* GetTarget() = 0;
	virtual void TurnToTarget(FVector Position) = 0;
	virtual void InitState() = 0;
	virtual void Attack() = 0;

	virtual FOnChangeStateDelegate& GetStateDelegate() = 0;
	virtual void SetAIAttackDelegate(FOnAttackEndDelegate& OnAttackEndDelegate) = 0;

};
