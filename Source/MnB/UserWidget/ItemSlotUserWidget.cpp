// Fill out your copyright notice in the Description page of Project Settings.


#include "UserWidget/ItemSlotUserWidget.h"
#include "Components/Image.h"
#include "Components/Button.h"

void UItemSlotUserWidget::NativeConstruct()
{
	Super::NativeConstruct();

	Button->OnHovered.AddDynamic(this, &ThisClass::OnItemBtnHovered);
	Button->OnClicked.AddDynamic(this, &ThisClass::OnItemBtnClicked);
	Button->OnReleased.AddDynamic(this, &ThisClass::OnItemBtnReleased);
	Button->OnPressed.AddDynamic(this, &ThisClass::OnItemBtnPressed);
}

void UItemSlotUserWidget::NativeOnInitialized()
{
	Super::NativeOnInitialized();
	Image = Cast<UImage>(GetWidgetFromName(TEXT("ItemImage")));
	Button = Cast<UButton>(GetWidgetFromName(TEXT("ItemButton")));
}

void UItemSlotUserWidget::OnItemBtnHovered()
{
	if (ItemBtnHovered.IsBound())
	{
		ItemBtnHovered.Execute(this);
	}
}

void UItemSlotUserWidget::OnItemBtnClicked()
{
	if (ItemBtnClicked.IsBound())
	{
		ItemBtnClicked.Execute(this);
	}
}

void UItemSlotUserWidget::OnItemBtnReleased()
{
	if (ItemBtnReleased.IsBound())
	{
		ItemBtnReleased.Execute(this);
	}
}

void UItemSlotUserWidget::OnItemBtnPressed()
{
	if (ItemBtnPressed.IsBound())
	{
		ItemBtnPressed.Execute(this);
	}
}

void UItemSlotUserWidget::SetImage(UTexture2D* InImage)
{
	Image->SetBrushFromTexture(InImage, false);
}
