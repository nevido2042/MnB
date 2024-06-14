// Fill out your copyright notice in the Description page of Project Settings.


#include "Subsystem/DataSubsystem.h"

UDataSubsystem::UDataSubsystem()
{
	{
		static ConstructorHelpers::FObjectFinder<UDataTable> Asset{ TEXT("/Script/Engine.DataTable'/Game/MyAssets/Item/DT_ItemData.DT_ItemData'") };
		ensure(Asset.Object);
		ItemDataTable = Asset.Object;
	}
}

const FItemData* UDataSubsystem::FindItem(const FName& InKey)
{
	FItemData* Row = ItemDataTable->FindRow<FItemData>(InKey, TEXT(""));
	ensure(Row);
	return Row;
}
