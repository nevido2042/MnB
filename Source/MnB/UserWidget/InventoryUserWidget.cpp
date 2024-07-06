// Fill out your copyright notice in the Description page of Project Settings.


#include "UserWidget/InventoryUserWidget.h"
#include "Components/UniformGridPanel.h"
#include "UserWidget/ItemSlotUserWidget.h"
#include "Subsystem/InventorySubsystem.h"
#include "ItemSlotUserWidget.h"
#include "Components/Image.h"
#include "Data/Item/ItemData.h"
#include "Item/Item.h"

void UInventoryUserWidget::NativeConstruct()
{
	Super::NativeConstruct();

	/*LoadClass<UClass>(ANY_PACKAGE, TEXT("/Script/UMGEditor.WidgetBlueprint'/Game/MyAssets/UI/BP_ItemSlot.BP_ItemSlot_C'"),
		nullptr, LOAD_None, nullptr);*/

	/*UClass* ItemSlotAsset = FindObject<UClass>(ANY_PACKAGE, TEXT("/Script/UMGEditor.WidgetBlueprint'/Game/MyAssets/UI/BP_ItemSlot.BP_ItemSlot_C'"));*/

	Panel = Cast<UUniformGridPanel>(GetWidgetFromName(TEXT("InventoryPanel")));

	ChestSlot = Cast<UItemSlotUserWidget>(GetWidgetFromName(TEXT("UI_ChestSlot")));
	ChestSlot->SetImage(nullptr);

	//BindEvent(ChestSlot);
	ChestSlotBindEvent(ChestSlot);

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

			BindEvent(ItemSlot);

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

	SetVisibility(ESlateVisibility::Hidden);
	//LeftImage = Cast<UImage>(GetWidgetFromName(TEXT("ImageLeft")));

	/*ChestSlot->ItemBtnClicked.BindLambda([this]()
	{
		ChestSlot->SetImage(nullptr);
		InventorySubsystem->AddItem("Chest");
	});*/
}

void UInventoryUserWidget::FlushInven()
{
	if (InventorySubsystem->Chest)
	{
		UTexture2D* Texture = InventorySubsystem->Chest->ItemImage;
		ChestSlot->Image->SetBrushFromTexture(Texture, false);
	}
	else
	{
		ChestSlot->Image->SetBrushFromTexture(nullptr, false);
	}

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

void UInventoryUserWidget::BindEvent(UItemSlotUserWidget* ItemSlot)
{
	ItemSlot->ItemBtnHovered.BindLambda([this](UItemSlotUserWidget* InSlot)
		{
			LastHoveredIndex = InSlot->ItemIndex;

			UE_LOG(LogTemp, Warning, TEXT("Hovered %d"), LastHoveredIndex);

			if (bHoldItem)
			{
				TSharedPtr<FItemData> Temp = InventorySubsystem->Inventory[PressedIndex];
				InventorySubsystem->Inventory[PressedIndex] = InventorySubsystem->Inventory[LastHoveredIndex];
				InventorySubsystem->Inventory[LastHoveredIndex] = Temp;

				bHoldItem = false;
				FlushInven();
			}
		});

	ItemSlot->ItemBtnPressed.BindLambda([this](UItemSlotUserWidget* InSlot)
		{
			PressedIndex = InSlot->ItemIndex;
			UE_LOG(LogTemp, Warning, TEXT("Pressed %d"), PressedIndex);

			bHoldItem = true;
		});

	//test
	ItemSlot->ItemBtnClicked.BindLambda([this](UItemSlotUserWidget* InSlot)
		{
			UE_LOG(LogTemp, Warning, TEXT("Clicked %d"), InSlot->ItemIndex);
			bHoldItem = false;

			if (InventorySubsystem->Inventory[InSlot->ItemIndex] == nullptr) return;


			//InventorySubsystem->Inventory[InSlot->ItemIndex]->
			const uint32 Index = InSlot->ItemIndex;

			TWeakPtr<FItemData> ItemData = InventorySubsystem->Inventory[Index];
			if (ItemData.IsValid())
			{
				if (ItemData.Pin()->ItemFunctionClass = UItem_Chest::StaticClass())
				{
					ChestSlot->SetImage(InventorySubsystem->Inventory[InSlot->ItemIndex]->ItemImage);
				}

				InventorySubsystem->UseItem(this, Index);
			}

			//InventorySubsystem->Inventory[InSlot->ItemIndex] = nullptr;
			FlushInven();
		});
}

void UInventoryUserWidget::ChestSlotBindEvent(UItemSlotUserWidget* ItemSlot)
{
	ItemSlot->ItemBtnClicked.BindLambda([this](UItemSlotUserWidget* InSlot)
		{
			/*if (InSlot->Image == nullptr)
				return;*/

			InventorySubsystem->UnEquipChest(this);
			/*InSlot->SetImage(nullptr);*/
			//InventorySubsystem->AddItem("Chest");
			/*InventorySubsystem->*/
			//FlushInven();
		});
}
