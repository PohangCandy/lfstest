// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/ASMainGameWidget.h"
#include "Components/ProgressBar.h"
#include "Components/EditableTextBox.h"
#include "Components/TextBlock.h"
#include "Player/ASPlayerState.h"
#include "Character/ASCharacterBase.h"


void UASMainGameWidget::BindPlayerBase(class AASCharacterBase* PlayerBase)
{
	CurrentPlayerBase = PlayerBase;
	CurrentPlayerBase->OnHpChanged.AddUObject(this, &UASMainGameWidget::UpdatePlayerState);
}

void UASMainGameWidget::BindPlayerBaseForBullet(AASCharacterBase* PlayerBase)
{
	CurrentPlayerBase = PlayerBase;
	CurrentPlayerBase->NumBulletChanged.AddUObject(this, &UASMainGameWidget::UpdateBulletUI);
}

void UASMainGameWidget::BindPlayerBaseForMagazine(AASCharacterBase* PlayerBase)
{
	CurrentPlayerBase = PlayerBase;
	CurrentPlayerBase->NumMagazineChanged.AddUObject(this, &UASMainGameWidget::UpdateMagazineUI);
}

void UASMainGameWidget::BindPlayerBaseForItem(AASCharacterBase* PlayerBase)
{
	CurrentPlayerBase = PlayerBase;
	CurrentPlayerBase->NumItemChanged.AddUObject(this, &UASMainGameWidget::UpdateItemUI);
}

void UASMainGameWidget::BindPlayerBaseForMagnification(AASCharacterBase* PlayerBase)
{
	CurrentPlayerBase = PlayerBase;
	CurrentPlayerBase->NumMagnificationChanged.AddUObject(this, &UASMainGameWidget::UpdateMagnificationUI);
}



void UASMainGameWidget::NativeConstruct()
{
	Super::NativeConstruct();
	HpBar = Cast<UProgressBar>(GetWidgetFromName(TEXT("PB_HP")));
	BulletNumUI = Cast<UEditableTextBox>(GetWidgetFromName(TEXT("LastBulletNum")));
	MagazineNum = Cast<UEditableTextBox>(GetWidgetFromName(TEXT("TB_LastMagazineNum")));
	ItemNum = Cast<UTextBlock>(GetWidgetFromName(TEXT("T_LastItem")));
	MagnificationNum = Cast<UTextBlock>(GetWidgetFromName(TEXT("Num_Magnification")));
	MagnificationBar = Cast<UProgressBar>(GetWidgetFromName(TEXT("PB_Magnification")));
}

void UASMainGameWidget::UpdatePlayerState()
{
	if (HpBar != nullptr)
	//HpBar->SetPercent(CurrentPlayerState->GetPlayerHpratio());
	HpBar->SetPercent(CurrentPlayerBase->GetHpratio());
}

void UASMainGameWidget::UpdateBulletUI()
{
	if (BulletNumUI != nullptr)
	BulletNumUI->SetText(FText::FromString(FString::FromInt(CurrentPlayerBase->GetBulletNum())));
}

void UASMainGameWidget::UpdateMagazineUI()
{
	if (MagazineNum != nullptr)
	MagazineNum->SetText(FText::FromString(FString::FromInt(CurrentPlayerBase->GetMagazineNum())));
}

void UASMainGameWidget::UpdateItemUI()
{
	if (ItemNum != nullptr)
	ItemNum->SetText(FText::FromString(FString::FromInt(CurrentPlayerBase->GetItemNum())));
}

void UASMainGameWidget::UpdateMagnificationUI()
{
	if(MagnificationNum != nullptr)
	MagnificationNum->SetText(FText::FromString(FString::FromInt(CurrentPlayerBase->GetMagnificationNum())));

	if (MagnificationBar != nullptr)
		MagnificationBar->SetPercent(CurrentPlayerBase->GetMagnificationratio());
}


