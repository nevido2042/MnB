// Fill out your copyright notice in the Description page of Project Settings.


#include "UserWidget/ControlUnitWidget.h"
#include "Components/CanvasPanel.h"
#include "Components/TextBlock.h"

void UControlUnitWidget::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	SwordManPanel = Cast<UCanvasPanel>(GetWidgetFromName(TEXT("UI_SwordMan")));
	BowManPanel = Cast<UCanvasPanel>(GetWidgetFromName(TEXT("UI_BowMan")));

	SwordManCount = Cast<UTextBlock>(GetWidgetFromName(TEXT("UI_SwordManCount")));
	BowManCount = Cast<UTextBlock>(GetWidgetFromName(TEXT("UI_BowManCount")));

	ActiveSwordManPanel(false);
	ActiveBowManPanel(false);
}

void UControlUnitWidget::ActiveSwordManPanel(bool Value, uint8 Count)
{
	if (Value)
	{
		SetSwordManCount(Count);
		SwordManPanel->SetVisibility(ESlateVisibility::Visible);
	}
	else
	{
		SwordManPanel->SetVisibility(ESlateVisibility::Hidden);
	}
}

void UControlUnitWidget::ActiveBowManPanel(bool Value, uint8 Count)
{
	if (Value)
	{
		SetBowManCount(Count);
		BowManPanel->SetVisibility(ESlateVisibility::Visible);

	}
	else
	{
		BowManPanel->SetVisibility(ESlateVisibility::Hidden);

	}
}

void UControlUnitWidget::SetSwordManCount(uint8 Count)
{
	SwordManCount->SetText(FText::Format(FTextFormat::FromString("{0}"), Count));
}

void UControlUnitWidget::SetBowManCount(uint8 Count)
{
	BowManCount->SetText(FText::Format(FTextFormat::FromString("{0}"), Count));
}
