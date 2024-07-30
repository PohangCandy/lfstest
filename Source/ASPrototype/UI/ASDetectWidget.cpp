// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/ASDetectWidget.h"
#include "Components/ProgressBar.h"
#include "Math/UnrealMathUtility.h"
#include "Components/WidgetComponent.h"
#include "Interface/ASAIWidgetInterface.h"


void UASDetectWidget::NativeConstruct()
{
	Super::NativeConstruct();
	DetectBar = Cast<UProgressBar>(GetWidgetFromName(TEXT("DetectProgressBar")));
	ensure(DetectBar);


	IsDecrease = false;
	Max = 1.0f;
	Min = 0.0f;
	Speed = 0.5;

	if (CurveFloat)
	{
		FOnTimelineFloat TimelineProgress;
		TimelineProgress.BindDynamic(this, &UASDetectWidget::UpdateDetection);
		Timeline.AddInterpFloat(CurveFloat, TimelineProgress);
		ensure(CurveFloat);
	}
	
	DetectBar->SetVisibility(ESlateVisibility::Hidden);
	AddToViewport();

	
	IASAIWidgetInterface* Enemy = Cast<IASAIWidgetInterface>(Owner);
	if (Enemy)
	{
		UASUserWidget* Widget = Cast<UASUserWidget>(this); 
		Enemy->SetupDetectWidget(Widget);
	}
}

void UASDetectWidget::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);
	Timeline.TickTimeline(InDeltaTime);
}

void UASDetectWidget::WidgetOn()
{
	DetectBar->SetVisibility(ESlateVisibility::Visible);
	Timeline.Play();
}

void UASDetectWidget::WidgetOff()
{
	DetectBar->SetVisibility(ESlateVisibility::Hidden);
	Timeline.Stop();
}


void UASDetectWidget::BlinkBar()
{
	SetRedColor();
	EndDelegate.BindDynamic(this, &UASDetectWidget::WidgetOff);
	if (Blink == nullptr) return;
	BindToAnimationFinished(Blink, EndDelegate);
	PlayAnimation(Blink, Min, 3);
	FullPercentDelegate.Broadcast();
}


void UASDetectWidget::SetRedColor()
{
	DetectBar->SetFillColorAndOpacity(FLinearColor::Red);
}

void UASDetectWidget::SetPercent(float f)
{
	DetectBar->SetPercent(f);
}

void UASDetectWidget::SetAngle(float angle)
{
	SetRenderTransformAngle(angle);
}


void UASDetectWidget::IncreaseDetection()
{
	WidgetOn();
	IsDecrease = false;
}

void UASDetectWidget::DecreaseDetection()
{
	Timeline.Reverse();
	IsDecrease = true;
}

void UASDetectWidget::UpdateDetection(float value)
{

	CurPercent = FMath::Lerp(Min, Max, value);
	GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Red, FString::Printf(TEXT("CurPercent : %f"),CurPercent));

	if (CurPercent >= Max) { BlinkBar(); return; }
	if (IsDecrease && value >= 0.7) { WidgetOff(); AlertDelegate.Broadcast(); return; }
	if (IsDecrease && value <= Min) { WidgetOff(); return; }
	DetectBar->SetPercent(CurPercent);
}














