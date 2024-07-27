// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/ASWidgetComponent.h"
#include "UI/ASUserWidget.h"
void UASWidgetComponent::InitWidget()
{
	Super::InitWidget();
	UASUserWidget* UserWidget = Cast<UASUserWidget>(GetWidget());
	if (UserWidget == NULL) { return; }
	UserWidget->SetOwningActor(GetOwner());
}

