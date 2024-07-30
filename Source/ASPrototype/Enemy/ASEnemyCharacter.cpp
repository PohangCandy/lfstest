// Fill out your copyright notice in the Description page of Project Settings.


#include "Enemy/ASEnemyCharacter.h"
#include "AI/ASAIController.h"
#include "DrawDebugHelpers.h"
#include "Kismet/GameplayStatics.h"
#include "Engine/DamageEvents.h"
#include "Interface/ASCharacterInterface.h"

AASEnemyCharacter::AASEnemyCharacter()
{
	AIControllerClass = AASAIController::StaticClass();
	AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;
}

float AASEnemyCharacter::TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser)
{
	Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);
	if (GetHp() <= 0)
	{
		return DamageAmount;
	}
	PlaySound(HitSound);
	PlayHitReactAnimation();
	
	SetHp(GetHp()-DamageAmount);
	return DamageAmount;
}

void AASEnemyCharacter::Tick(float DeltaTime)
{

	Super::Tick(DeltaTime);
}

void AASEnemyCharacter::AttackCheck()
{
	Super::AttackCheck();
	FHitResult OutHit;
	FDamageEvent DamageEvent;
	FVector Start = GetActorLocation();//aponInfo->WeaponModel->GetComponentLocation();
	FVector ForwardVector = GetActorForwardVector();
	FVector End = (Start + (ForwardVector * 1000.0f));

	FCollisionQueryParams CollisionParams(SCENE_QUERY_STAT(Attack), false, this);

	//DrawDebugLine(GetWorld(), Start, End, FColor::Green, false, 1, 0, 1);
	bool isHit = GetWorld()->LineTraceSingleByChannel(OutHit, Start, End, ECC_GameTraceChannel4, CollisionParams); //   EnemyAttack 
	if (!isHit) return;
	IASCharacterInterface* Player = Cast<IASCharacterInterface>(OutHit.GetActor());
	if (Player)
	{
		OutHit.GetActor()->TakeDamage(10.0f, DamageEvent,GetController(),this);
	}
	else
	{
		GEngine->AddOnScreenDebugMessage(-1, 1.f, FColor::Red, FString::Printf(TEXT(" hitting: Others ")));
	}
}

void AASEnemyCharacter::BeginPlay()
{
	Super::BeginPlay();
}






