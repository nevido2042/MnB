// Fill out your copyright notice in the Description page of Project Settings.


#include "UserWidget/InventoryUserWidget.h"
#include "Components/UniformGridPanel.h"
#include "UserWidget/ItemSlotUserWidget.h"

void UInventoryUserWidget::NativeConstruct()
{
	Super::NativeConstruct();

	LoadClass<UClass>(ANY_PACKAGE, TEXT("/Script/UMGEditor.WidgetBlueprint'/Game/MyAssets/UI/BP_ItemSlot.BP_ItemSlot_C'"),
		nullptr, LOAD_None, nullptr);
	UClass* ItemSlotAsset = FindObject<UClass>(ANY_PACKAGE, TEXT("/Script/UMGEditor.WidgetBlueprint'/Game/MyAssets/UI/BP_ItemSlot.BP_ItemSlot_C'"));

	Panel = Cast<UUniformGridPanel>(GetWidgetFromName(TEXT("InventoryPanel")));

	for (int i = 0; i < 5; i++)
	{
		for (int k = 0; k < 10; k++)
		{
			UItemSlotUserWidget* ItemSlot = Cast<UItemSlotUserWidget>(CreateWidget(this, ItemSlotAsset));
			ensure(ItemSlot);

			ItemSlot->SetImage(nullptr);

			Panel->AddChildToUniformGrid(ItemSlot, i, k);

		}
	}
}

void UInventoryUserWidget::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	//LeftImage = Cast<UImage>(GetWidgetFromName(TEXT("ImageLeft")));

}
