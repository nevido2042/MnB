// Fill out your copyright notice in the Description page of Project Settings.


#include "UserWidget/ItemSlotUserWidget.h"
#include "Components/Image.h"
#include "Components/Button.h"

void UItemSlotUserWidget::NativeConstruct()
{
	Super::NativeConstruct();

}

void UItemSlotUserWidget::NativeOnInitialized()
{
	Super::NativeOnInitialized();
	Image = Cast<UImage>(GetWidgetFromName(TEXT("ItemImage")));
	Button = Cast<UButton>(GetWidgetFromName(TEXT("ItemButton")));
}

void UItemSlotUserWidget::SetImage(UTexture2D* InImage)
{
	Image->SetBrushFromTexture(InImage, false);
}
