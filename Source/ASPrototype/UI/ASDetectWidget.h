// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/ASUserWidget.h"
#include "Components/TimelineComponent.h"
#include "ASDetectWidget.generated.h"

//DECLARE_MULTICAST_DELEGATE_OneParam(FUASDetectWidgetOnChangedSignature, bool b)
/**
 * 
 */

DECLARE_MULTICAST_DELEGATE(FOnFullPercentDelegate);
DECLARE_MULTICAST_DELEGATE(FOnWidgetTriggerDelegate);
DECLARE_MULTICAST_DELEGATE(FOnAlertDelegate);

UCLASS()
class ASPROTOTYPE_API UASDetectWidget : public UASUserWidget
{
	GENERATED_BODY()

public:
	FWidgetAnimationDynamicEvent EndDelegate;
	FOnFullPercentDelegate FullPercentDelegate;
	FOnAlertDelegate AlertDelegate;
	UFUNCTION()
	void WidgetOff();

	void WidgetOn();
	void SetRedColor();
	void SetPercent(float f);


	void BlinkBar();

	void SetAngle(float angle);

	void IncreaseDetection();
	void DecreaseDetection();

	UFUNCTION()
	void UpdateDetection(float value);

	UPROPERTY(EditAnywhere ,BlueprintReadWrite ,meta = (BindWidgetAnim), Transient)
	class UWidgetAnimation* Blink;

	UPROPERTY(EditAnywhere, Category= Speed )
	float Speed;

	UPROPERTY(EditAnywhere)
	UCurveFloat* CurveFloat;

	UPROPERTY(EditAnywhere)
	TObjectPtr<class UProgressBar> DetectBar;

protected:
	virtual void NativeConstruct() override;
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime);


private:
	//타임라인
	FTimeline Timeline;
	float Max;
	float Min;
	float CurPercent;
	bool IsDecrease;
};
