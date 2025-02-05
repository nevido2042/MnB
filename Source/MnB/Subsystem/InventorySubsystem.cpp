// Fill out your copyright notice in the Description page of Project Settings.


#include "Subsystem/InventorySubsystem.h"
#include "Data/Item/ItemData.h"
#include "DataSubsystem.h"
#include "UserWidget/InventoryUserWidget.h"
#include "Item/Item.h"

void UInventorySubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);
}

void UInventorySubsystem::MakeInventory()
{
	DataSubsystem = GetWorld()->GetGameInstance()->GetSubsystem<UDataSubsystem>();
	Inventory.SetNum(MaxInvenSize, false);
}

void UInventorySubsystem::ClearInventory()
{
	Inventory.Empty();
}

bool UInventorySubsystem::AddItem(const FName& InKey)
{
	const FItemData* Data = DataSubsystem->FindItem(InKey);
	if (!Data)
	{
		check(false);
		return false;
	}
	TSharedPtr<FItemData> NewItemData = MakeShared<FItemData>(*Data);
	NewItemData->ItemName = InKey;

	return MoveItemToInventory(NewItemData);
}

bool UInventorySubsystem::MoveItemToInventory(TSharedPtr<FItemData>& InItem)
{
	for (uint32 i = 0; i < MaxInvenSize; ++i)
	{
		TSharedPtr<FItemData> ItemData = Inventory[i];
		if (ItemData == nullptr) { continue; }
		if (ItemData->ItemName != InItem->ItemName) { continue; }
	}

	bool bAdded = false;
	for (uint32 i = 0; i < MaxInvenSize; ++i)
	{
		if (Inventory[i] == nullptr)
		{
			Inventory[i] = InItem;
			bAdded = true;
			break;
		}
	}

	InItem = nullptr;

	return bAdded;
}

void UInventorySubsystem::UnEquipChest(UInventoryUserWidget* Widget)
{
	if (Chest == nullptr) return;

	UItem* Item = Chest->ItemFunctionClass->GetDefaultObject<UItem>();
	Item->UnEquipItem(Cast<AController>(Widget->GetOwningPlayer()), *Chest);
	MoveItemToInventory(Chest);
	Widget->FlushInven();
}

void UInventorySubsystem::UnEquipHead(UInventoryUserWidget* Widget)
{
	if (Head == nullptr) return;

	UItem* Item = Head->ItemFunctionClass->GetDefaultObject<UItem>();
	Item->UnEquipItem(Cast<AController>(Widget->GetOwningPlayer()), *Head);
	MoveItemToInventory(Head);
	Widget->FlushInven();
}

void UInventorySubsystem::UseItem(UInventoryUserWidget* Widget, uint32 InIndex)
{
	TWeakPtr<FItemData> ItemData = Inventory[InIndex];
	if (!ItemData.IsValid()) { return; }

	AController* PlayerController = Cast<AController>(Widget->GetOwningPlayer());
	ensure(PlayerController);

	UItem* Item = ItemData.Pin()->ItemFunctionClass->GetDefaultObject<UItem>();

	UItem_Chest* ItemChest = Cast<UItem_Chest>(Item);
	if (ItemChest)
	{
		if (Chest)
		{
			MoveItemToInventory(Chest);
		}

		Item->UseItem(PlayerController, *ItemData.Pin());
		Chest = Inventory[InIndex];
		Inventory[InIndex] = nullptr;
	}

	UItem_Head* ItemHead = Cast<UItem_Head>(Item);
	if (ItemHead)
	{
		if (Head)
		{
			MoveItemToInventory(Head);
		}

		Item->UseItem(PlayerController, *ItemData.Pin());
		Head = Inventory[InIndex];
		Inventory[InIndex] = nullptr;
	}
	/*UItem_Weapon* ItemWeapon = Cast<UItem_Weapon>(Item);
	if (ItemWeapon)
	{
		Weapon = Inventory[InIndex];
		Item->UseItem(RPGPlayerController, *ItemData.Pin());
		Inventory[InIndex] = nullptr;
	}
	else
	{
		Item->UseItem(RPGPlayerController, *ItemData.Pin());
		--ItemData.Pin()->CurrentBundleCount;
	}

	if (ItemData.Pin()->CurrentBundleCount == 0)
	{
		Inventory[InIndex] = nullptr;
	}*/

	Widget->FlushInven();
}
