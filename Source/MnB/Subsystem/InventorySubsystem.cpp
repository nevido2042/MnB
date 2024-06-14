// Fill out your copyright notice in the Description page of Project Settings.


#include "Subsystem/InventorySubsystem.h"
#include "Data/Item/ItemData.h"
#include "DataSubsystem.h"

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
