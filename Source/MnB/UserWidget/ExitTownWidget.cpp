// Fill out your copyright notice in the Description page of Project Settings.


#include "UserWidget/ExitTownWidget.h"
#include "Components/ProgressBar.h"

void UExitTownWidget::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	ExitTownBar = Cast<UProgressBar>(GetWidgetFromName(TEXT("UI_ProgressBar")));
}
