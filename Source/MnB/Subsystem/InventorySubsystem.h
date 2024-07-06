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

	friend class UInventoryUserWidget;
	friend class AControllerPC;
protected:
	/** Implement this for initialization of instances of the system */
	virtual void Initialize(FSubsystemCollectionBase& Collection);

	void MakeInventory();
	void ClearInventory();


	bool MoveItemToInventory(TSharedPtr<FItemData>& InItem);


	void UseItem(class UInventoryUserWidget* Widget, uint32 InIndex);

	void UnEquipChest(UInventoryUserWidget* Widget);
	void UnEquipHead(UInventoryUserWidget* Widget);

public:
	bool AddItem(const FName& InKey);

protected:
	const uint32 MaxInvenSize = 20;
	TArray<TSharedPtr<FItemData>> Inventory;

	TSharedPtr<FItemData> Chest;
	TSharedPtr<FItemData> Head;

	class UDataSubsystem* DataSubsystem;
};
