// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Enemy/ASEnemyBase.h"
#include "Interface/ASAttackCheckInterface.h"
#include "ASEnemyCharacter.generated.h"
/**
 * 
 */
UCLASS()
class ASPROTOTYPE_API AASEnemyCharacter : public AASEnemyBase, public IASAttackCheckInterface
{
	GENERATED_BODY()
public:
	AASEnemyCharacter();

	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser) override;
	virtual void Tick(float DeltaTime) override;

protected:
	virtual void BeginPlay() override;
	virtual void AttackCheck() override;
};
