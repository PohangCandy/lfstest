// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/ASUserWidget.h"
#include "Blueprint/UserWidget.h"
#include "ASQuestionMarkWidget.generated.h"

/**
 * 
 */
UCLASS()
class ASPROTOTYPE_API UASQuestionMarkWidget : public UASUserWidget
{
	GENERATED_BODY()
protected:

	virtual void NativeConstruct() override;
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime);

	UPROPERTY()
	TObjectPtr<class UImage> QuestionMark;

};
