// Fill out your copyright notice in the Description page of Project Settings.


#include "UserWidget/VRHealthWidget.h"
#include "Components/ProgressBar.h"

void UVRHealthWidget::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	UI_HeathBar = Cast<UProgressBar>(GetWidgetFromName(TEXT("HealthBar")));
}

void UVRHealthWidget::SetHealthBar(float Value)
{
	UI_HeathBar->SetPercent(Value);
}
