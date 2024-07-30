// Fill out your copyright notice in the Description page of Project Settings.


#include "Enemy/ASFinalTarget.h"
#include "Game/ASGameMode.h"
#include "Animation/ASAIAnimInstance.h"
AASFinalTarget::AASFinalTarget()
{

}


void AASFinalTarget::BeginPlay()
{
	CurGameMode = Cast<AASGameMode>(UGameplayStatics::GetGameMode(GetWorld()));
}


