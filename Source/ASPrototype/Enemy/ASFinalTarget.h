// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Enemy/ASEnemyCharacter.h"
#include "ASFinalTarget.generated.h"

/**
 * 
 */
UCLASS()
class ASPROTOTYPE_API AASFinalTarget : public AASEnemyCharacter
{
	GENERATED_BODY()
	
public:
	AASFinalTarget();
	virtual void SetDead() override;
	class AASGameMode* CurGameMode;

	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Animation)
	TObjectPtr<class UAnimMontage> BossDeathMontage;
};
