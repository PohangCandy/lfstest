// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/Eqs/EnvQueryContext_Player.h"
#include "AIController.h"
#include "EnvironmentQuery/EnvQueryTypes.h" //FEnvQueryInstance Æ÷ÇÔ
#include "EnvironmentQuery/Items/EnvQueryItemType_Actor.h" 

void UEnvQueryContext_Player::ProvideContext(FEnvQueryInstance& QueryInstance, FEnvQueryContextData& ContextData) const
{	
	Super::ProvideContext(QueryInstance, ContextData);
	AAIController * AI = Cast<AAIController>((Cast<AActor>((QueryInstance.Owner).Get())->GetInstigatorController()));
	if (AI)
	{
//		UEnvQueryItemType_Actor::SetContextHelper(ContextData, AIRef->GetPlayer());
	}

}

