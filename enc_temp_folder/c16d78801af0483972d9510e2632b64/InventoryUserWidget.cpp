// Fill out your copyright notice in the Description page of Project Settings.


#include "UserWidget/InventoryUserWidget.h"
#include "Components/UniformGridPanel.h"
#include "UserWidget/ItemSlotUserWidget.h"
#include "Subsystem/InventorySubsystem.h"
#include "ItemSlotUserWidget.h"
#include "Components/Image.h"
#include "Data/Item/ItemData.h"

void UInventoryUserWidget::NativeConstruct()
{
	Super::NativeConstruct();

	LoadClass<UClass>(ANY_PACKAGE, TEXT("/Script/UMGEditor.WidgetBlueprint'/Game/MyAssets/UI/BP_ItemSlot.BP_ItemSlot_C'"),
		nullptr, LOAD_None, nullptr);
	UClass* ItemSlotAsset = FindObject<UClass>(ANY_PACKAGE, TEXT("/Script/UMGEditor.WidgetBlueprint'/Game/MyAssets/UI/BP_ItemSlot.BP_ItemSlot_C'"));

	Panel = Cast<UUniformGridPanel>(GetWidgetFromName(TEXT("InventoryPanel")));


	ULocalPlayer* LocalPlayer = GetOwningLocalPlayer();
	InventorySubsystem = ULocalPlayer::GetSubsystem<UInventorySubsystem>(LocalPlayer);
	InvenSize = InventorySubsystem->Inventory.Num();
	Slots.Reserve(InvenSize);

	int32 Col = 5;
	int32 Row = InvenSize / Col;

	for (int i = 0; i < Row; i++)
	{
		for (int k = 0; k < Col; k++)
		{
			UItemSlotUserWidget* ItemSlot = Cast<UItemSlotUserWidget>(CreateWidget(this, ItemSlotAsset));
			ensure(ItemSlot);

			ItemSlot->ItemBtnHovered.BindLambda([this](UItemSlotUserWidget* InSlot) 
				{
					LastHoveredIndex = InSlot->ItemIndex;
				});

			ItemSlot->ItemBtnClicked.BindLambda([this](UItemSlotUserWidget* InSlot)
				{
					ClickedIndex = InSlot->ItemIndex;
				});

			ItemSlot->ItemBtnReleased.BindLambda([this](UItemSlotUserWidget* InSlot) 
				{
					/*Slots[LastHoveredIndex]->ItemIndex = ClickedIndex;
					Slots[ClickedIndex]->ItemIndex = LastHoveredIndex;*/
					FlushInven();
				});

			ItemSlot->ItemIndex = k + i * Col;

			Slots.Add(ItemSlot);

			ItemSlot->SetImage(nullptr);

			Panel->AddChildToUniformGrid(ItemSlot, i, k);

		}
	}

	FlushInven();
}

void UInventoryUserWidget::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	//LeftImage = Cast<UImage>(GetWidgetFromName(TEXT("ImageLeft")));

}

void UInventoryUserWidget::FlushInven()
{
	for (int32 i = 0; i < InvenSize; ++i)
	{
		if (InventorySubsystem->Inventory[i] == nullptr)
		{
			Slots[i]->Image->SetBrushFromTexture(nullptr, false);
			continue;
		}

		UTexture2D* Texture = InventorySubsystem->Inventory[i]->ItemImage;
		Slots[i]->Image->SetBrushFromTexture(Texture, false);
	}
}
