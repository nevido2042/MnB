// Fill out your copyright notice in the Description page of Project Settings.


#include "UserWidget/BattleSituationWidget.h"
#include "Components/ProgressBar.h"

void UBattleSituationWidget::NativeOnInitialized()
{
	Super::NativeOnInitialized();
	ATeamBar = Cast<UProgressBar>(GetWidgetFromName(TEXT("UI_ATeamBar")));
	BTeamBar = Cast<UProgressBar>(GetWidgetFromName(TEXT("UI_BTeamBar")));
}

void UBattleSituationWidget::SetATeamBarPercent(float Value)
{
	ATeamBar->SetPercent(Value);
}

void UBattleSituationWidget::SetBTeamBarPercent(float Value)
{
	BTeamBar->SetPercent(Value);
}
