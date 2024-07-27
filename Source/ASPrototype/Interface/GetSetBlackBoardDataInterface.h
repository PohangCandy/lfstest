// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "GetSetBlackBoardDataInterface.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UGetSetBlackBoardDataInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class ASPROTOTYPE_API IGetSetBlackBoardDataInterface
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	virtual void SetBB_LastKnownPosition(FVector vector) = 0;
	virtual FVector GetBB_LastKnownPosition() = 0;

	virtual void SetBB_Target(UObject* object) = 0;
	virtual UObject* GetBB_Target() = 0;

	virtual void SetBB_IsDetect(bool b) = 0;
	virtual bool GetBB_IsDetect() = 0;

	virtual void SetBB_IsAlert(bool b) = 0;
	virtual bool GetBB_IsAlert() = 0;

	virtual void SetBB_CanShootTarget(bool b) = 0;
	virtual bool GetBB_CanShootTarget() = 0;

	virtual void SetBB_PatrolLoc(FVector vector) = 0;
	virtual FVector GetBB_PatrolLoc() = 0;

	virtual void SetBB_PathLoc(FVector vector) = 0;
	virtual FVector GetBB_PathLoc() = 0;

	virtual void SetBB_AttackRange(FVector vector) = 0;
	virtual FVector GetBB_AttackRange() = 0;
};
