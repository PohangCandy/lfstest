// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/ASQuestionMarkWidget.h"
#include "Components/Image.h"
#include "Interface/ASAIWidgetInterface.h"

void UASQuestionMarkWidget::NativeConstruct()
{
	Super::NativeConstruct();
	QuestionMark = Cast<UImage>(GetWidgetFromName(TEXT("Question_Mark")));
	ensure(QuestionMark);
	SetVisibility(ESlateVisibility::Hidden);


	IASAIWidgetInterface* Enemy = Cast<IASAIWidgetInterface>(Owner);
	if (Enemy)
	{
		UASUserWidget* Widget = Cast<UASUserWidget>(this);
		Enemy->SetupAlertWidget(Widget);
	}
}

void UASQuestionMarkWidget::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);
}

