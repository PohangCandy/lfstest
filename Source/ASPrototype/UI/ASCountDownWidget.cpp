// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/ASCountDownWidget.h"
#include "Components/EditableText.h"
#include "Containers/UnrealString.h"
#include "Game/ASGameMode.h"
#include "Math/UnrealMathUtility.h"

//시작시 addtoviweport이후 끄기 ,켜기만 할거임

void UASCountDownWidget::NativeOnInitialized()
{
	Super::NativeOnInitialized();
	start = false;
	CountDown = 10.0f;
	Minus = 0.0f;
	curGameMode = Cast<AASGameMode>(UGameplayStatics::GetGameMode(GetWorld()));
	ensure(curGameMode);

	Timer = Cast<UEditableText>(GetWidgetFromName(TEXT("CountDownText")));
	ensure(Timer);
	Timer->SetText(FText::AsNumber(CountDown));
	//Timer->AddToRoot();
	//AddToViewport();
	//OffVisible();
}

void UASCountDownWidget::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);
	if (GetStart())
	{
		CountDown = CountDown - GetWorld()->GetDeltaSeconds();
		Timer->SetText(FText::AsNumber(FMath::Clamp(int(CountDown), 0, 10)));
		if (CountDown <= 0)
		{
			//start = false;
			curGameMode->GameOver();
		}
	}
}

//void UASCountDownWidget::NativeConstruct()
//{
//	Super::NativeConstruct();
//	Timer = Cast<UEditableText>(GetWidgetFromName(TEXT("CountDownText")));
//	ensure(Timer);
//	start = false;
//	CountDown = 10.0f;
//	Minus = 0.0f;
//	Timer->SetText(FText::AsNumber(CountDown));
//	curGameMode = Cast<AASGameMode>(UGameplayStatics::GetGameMode(GetWorld()));
//	ensure(curGameMode);
//	AddToViewport();
//	OffVisible();
//}
//


void UASCountDownWidget::ViewPort()
{
	if (this)
	{
		AddToViewport();
	}
}

void UASCountDownWidget::SetStart(bool setstart)
{
	AddToViewport();
	OnVisible();
	start = setstart;
}

bool UASCountDownWidget::GetStart()
{
	return start;
}

void UASCountDownWidget::OffVisible()
{
	Timer->SetVisibility(ESlateVisibility::Hidden);
}

void UASCountDownWidget::OnVisible()
{
	if (Timer)
	{
		Timer->SetVisibility(ESlateVisibility::Visible);
	}
}
