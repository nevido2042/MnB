// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/LocalPlayerSubsystem.h"
#include "InventorySubsystem.generated.h"

/**
 * 
 */

struct FItemData;

UCLASS()
class MNB_API UInventorySubsystem : public ULocalPlayerSubsystem
{
	GENERATED_BODY()

protected:
	void MakeInventory();
	void ClearInventory();

	bool AddItem(const FName& InKey);
	bool MoveItemToInventory(TSharedPtr<FItemData>& InItem);

protected:
	const uint32 MaxInvenSize = 60;
	TArray<TSharedPtr<FItemData>> Inventory;

	class UDataSubsystem* DataSubsystem;
};
