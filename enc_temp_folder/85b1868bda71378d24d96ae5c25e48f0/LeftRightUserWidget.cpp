// Fill out your copyright notice in the Description page of Project Settings.


#include "UserWidget/LeftRightUserWidget.h"
#include "Components/Image.h"

void ULeftRightUserWidget::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	ImageLeft = Cast<UImage>(GetWidgetFromName(TEXT("ImageLeft")));

	ImageRight = Cast<UImage>(GetWidgetFromName(TEXT("ImageRight")));

	FLinearColor NewColor = FLinearColor::Red;

	ImageLeft->SetColorAndOpacity(NewColor);
}
