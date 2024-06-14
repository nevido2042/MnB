// Fill out your copyright notice in the Description page of Project Settings.


#include "Controller/ControllerPC.h"
#include "Blueprint/UserWidget.h"
#include "Subsystem/InventorySubsystem.h"

AControllerPC::AControllerPC()
{
	{
		ConstructorHelpers::FClassFinder<UUserWidget>Finder(TEXT("/Script/UMGEditor.WidgetBlueprint'/Game/MyAssets/UI/BP_PCWidget.BP_PCWidget_C'"));
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
}

void AControllerPC::OnPossess(APawn* aPawn)
{
	Super::OnPossess(aPawn);

	UInventorySubsystem* InventorySubsystem = ULocalPlayer::GetSubsystem<UInventorySubsystem>(GetLocalPlayer());
	InventorySubsystem->MakeInventory();
}
