// Fill out your copyright notice in the Description page of Project Settings.


#include "UserWidget/InventoryUserWidget.h"
#include "Components/UniformGridPanel.h"

void UInventoryUserWidget::NativeConstruct()
{
	Super::NativeConstruct();

	LoadClass<UClass>(ANY_PACKAGE, TEXT("/Script/UMGEditor.WidgetBlueprint'/Game/MyAssets/UI/BP_ItemSlot.BP_ItemSlot_C'"),
		nullptr, LOAD_None, nullptr);
	UClass* ItemSlot = FindObject<UClass>(ANY_PACKAGE, TEXT("/Script/UMGEditor.WidgetBlueprint'/Game/MyAssets/UI/BP_ItemSlot.BP_ItemSlot_C'"));

	Panel = Cast<UUniformGridPanel>(GetWidgetFromName(TEXT("InventoryPanel")));

	for (int i = 0; i < 5; i++)
	{
		for (int k = 0; k < 10; k++)
		{
			UUserWidget* Widget = Cast<UUserWidget>(CreateWidget(this, ItemSlot));
			ensure(Widget);

			Panel->AddChildToUniformGrid(Widget, i, k);

		}
	}
}

void UInventoryUserWidget::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	//LeftImage = Cast<UImage>(GetWidgetFromName(TEXT("ImageLeft")));

}
