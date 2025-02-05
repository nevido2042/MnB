// Fill out your copyright notice in the Description page of Project Settings.


#include "Controller/ControllerPC.h"
#include "Blueprint/UserWidget.h"
#include "Subsystem/InventorySubsystem.h"
#include "UserWidget/InventoryUserWidget.h"

AControllerPC::AControllerPC()
{
	{
		static ConstructorHelpers::FClassFinder<UUserWidget>Finder(TEXT("/Script/UMGEditor.WidgetBlueprint'/Game/MyAssets/UI/BP_PCWidget.BP_PCWidget_C'"));
		ensure(Finder.Class);
		if (Finder.Class)
		{
			Widget = Finder.Class;
		}
	}

	
}

void AControllerPC::BeginPlay()
{
	CurrentWidget = CreateWidget(GetWorld(), Widget);
	CurrentWidget->AddToViewport();
	CreateInventoryWidget();

	SetShowMouseCursor(false);
	FInputModeGameOnly Mode;
	SetInputMode(Mode);

	ExitTownWidget = CreateWidget(GetWorld(),ExitTownWidgetAsset);
	ExitTownWidget->AddToViewport();
	ExitTownWidget->SetVisibility(ESlateVisibility::Hidden);

	ControlUnitWidget = CreateWidget(GetWorld(), ControlUnitWidgetAsset);
	ControlUnitWidget->AddToViewport();
	//ControlUnitWidget->SetVisibility(ESlateVisibility::Hidden);
}

void AControllerPC::OnPossess(APawn* aPawn)
{
	Super::OnPossess(aPawn);

	UInventorySubsystem* InventorySubsystem = ULocalPlayer::GetSubsystem<UInventorySubsystem>(GetLocalPlayer());
	InventorySubsystem->MakeInventory();

	InventorySubsystem->AddItem("BrownChest");
	InventorySubsystem->AddItem("BrownHead");
	InventorySubsystem->AddItem("WhiteChest");
	InventorySubsystem->AddItem("WhiteHead");
}

void AControllerPC::PickUpItem(const FName& InKey)
{
	UInventorySubsystem* InventorySubsystem = ULocalPlayer::GetSubsystem<UInventorySubsystem>(GetLocalPlayer());
	InventorySubsystem->AddItem(InKey);
	InventoryWidget->FlushInven();
}

void AControllerPC::CreateInventoryWidget()
{
	UUserWidget* InvenWidget = Cast<UUserWidget>(InventoryWidget);
	InvenWidget = CreateWidget(GetWorld(), InventoryWidgetAsset);
	InvenWidget->AddToViewport();

	InventoryWidget = Cast<UInventoryUserWidget>(InvenWidget);
}
